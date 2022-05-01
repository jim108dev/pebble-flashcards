namespace Application

module Main =

    open System.IO
    open Deedle
    open Domain.Types
    open Types

    open Domain

    module Util = DeedleUtil

    let alignHistory (c:Config) = 
        
        if  not (File.Exists c.history.file) then 
            c.reference.file |> Storage.copy c.history.file
        else
            let history = 
                c.history.file
                |> Storage.read
                |> Frame.indexRowsUsing (fun row -> row.GetAs<Id> C.id)

            let historyIds = history.ColumnKeys |> Set.ofSeq
            let referenceOuter =
                c.reference.file
                |> Storage.read
                |> Frame.indexRowsUsing (fun row -> row.GetAs<Id> C.id)
                |> Frame.filterRows (fun i _ -> historyIds.Contains i)
            
            history
            |> Frame.join JoinKind.Outer referenceOuter
            |> Storage.writeVerbose c.history.file

    let append x xs =
        [xs;x] |> String.concat "|"  

    let mergeFeedback (c:Config) =
        let feedback = 
            c.feedback.file
            |> Storage.read
            |> Frame.indexRowsUsing (fun row -> row.GetAs<Id> C.id)
        
        let history = 
            c.history.file
            |> Storage.read
            |> Frame.indexRowsUsing (fun row -> row.GetAs<Id> C.id)

        feedback
        |> Frame.join JoinKind.Outer history
        |> Util.mapRowValues C.feedbackHistory (fun row -> 
            row.GetAs<FeedbackHistory> C.feedbackHistory 
            |> append (string (row.GetAs<Feedback> C.feedback )))
        |> Util.mapRowValues C.startHistory (fun row -> 
            row.GetAs<StartHistory> C.startHistory 
            |> append (string (row.GetAs<Start> C.start )))
        |> Util.mapRowValues C.stopHistory (fun row -> 
            row.GetAs<StopHistory> C.stopHistory 
            |> append (string (row.GetAs<Stop> C.stop )))