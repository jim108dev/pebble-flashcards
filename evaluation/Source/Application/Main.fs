namespace Application

module Main =

    open System
    open Deedle
    open Domain.Types
    open Types

    open Domain
    open FSharpPlus.Data

    module Util = DeedleUtil

    let toDateTime (timestamp:int) =
        let start = DateTime(1970,1,1,0,0,0,DateTimeKind.Utc)
        start.AddSeconds(float timestamp).ToLocalTime()

    let mergeFeedback (c:Config) =
        let feedback = 
            c.feedback.file
            |> Storage.read
            |> Frame.indexRowsUsing (fun row -> row.GetAs<Id> C.id, row.GetAs<int> C.stop)

        let history = 
            c.history.file
            |> Storage.read
            |> Frame.indexRowsUsing (fun row -> row.GetAs<Id> C.id, row.GetAs<int> C.stop)

        try
           feedback
            |> Frame.merge history
            |> Storage.writeVerbose c.history.file
        with
        | :? InvalidOperationException -> printfn "Could not merge files. (Possibly already merged.)"


    let aggFunc (series:Series<Id*int,string>) :List<int> =
        series.Values 
        |> Seq.map int
        |> Seq.toList

    let private getWaitingPeriod (defaultPeriod:Period) (row:ObjectSeries<string>) :Period = 
        match row.TryGetAs<list<int>> C.feedback with
        | OptionalValue.Present answers ->
            match answers with
            | [] -> defaultPeriod
            | xs ->
                xs
                |> List.map enum<Answer> 
                |> NonEmptyList.ofList  
                |> SuperMemo2.getWaitingPeriod
        | OptionalValue.Missing -> defaultPeriod

    let private isActive (row:ObjectSeries<string>) :bool = 
        match row.TryGetAs<list<int>> C.feedback with
        | OptionalValue.Present answers ->
            match answers with
            | [] -> true
            | xs ->
                xs
                |> List.map enum<Answer> 
                |> Set.ofList  
                |> Set.contains Answer.Buried
                |> not
        | OptionalValue.Missing -> true

    let prepareNextSession (c:Config) =
        
        let reference = 
            c.reference.file
            |> Storage.read
            |> Frame.indexRowsUsing (fun row -> row.GetAs<Id> C.id)

        let history = 
            c.history.file
            |> Storage.read
            |> Frame.indexRowsUsing (fun row -> row.GetAs<Id> C.id, row.GetAs<int> C.stop)

        let aggregated =
            history
            |> Frame.aggregateRowsBy [C.id] [C.start;C.stop;C.feedback] aggFunc
            |> Frame.indexRowsUsing (fun row -> row.GetAs<Id> C.id)

        let selected = 
            aggregated.Columns.[[C.feedback]]
            |> Frame.join JoinKind.Outer reference
            |> Frame.filterRowValues isActive
            |> DeedleUtil.mapRowValues C.waitingPeriod (getWaitingPeriod c.next_session.new_record_period)
            |> Frame.sortRowsBy C.waitingPeriod id
            |> Frame.filterRowValues (fun row -> row.GetAs<Period> C.waitingPeriod < c.next_session.max_period)
            |> Frame.take c.next_session.max_records

        selected.Columns.[[C.id;C.text1;C.text2]]
        |> Storage.write c.next_session.file
