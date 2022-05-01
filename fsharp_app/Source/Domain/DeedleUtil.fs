namespace Domain

module DeedleUtil =

    open Deedle
    open Microsoft.FSharp.Reflection
    open System

    let appendColumn (column: string) (series: ISeries<'a>) (df: Frame<_, _>) =
        let df1 = df.Clone()

        df1.AddColumn(column, series)
        
        df1

    let appendConstColumn (column: string) value (df: Frame<_, _>) =
        let newCol =
            df |> Frame.mapRowValues (fun _ -> value)

        df |> Frame.addCol column newCol

    let mapColKey (c2c: Map<string, string>) name =
        match c2c.TryFind name with
        | Some c -> c
        | None -> name

    let mapColKeys (c2c: Map<string, string>) (df: Frame<_, _>) = 
        df
        |> Frame.mapColKeys (mapColKey c2c)

    let mapRowValue (column: string) (f: ('T -> 'R)) (df: Frame<_, string>) =
        let df1 = df.Clone()

        df1.ReplaceColumn(column, df.Columns.[column].As<'T>() |> Series.mapValues f)

        df1

    let mapRowValues (column: string) (f:ObjectSeries<string> -> 'V) (df: Frame<_, _>) =
        let df1 = df.Clone()

        df1.ReplaceColumn(column, df |> Frame.mapRowValues f)

        df1

    let mapAll (column: string) (f:('K -> option<'T> -> option<'R>))  (df: Frame<_, string>) =
        let df1 = df.Clone()

        df1.ReplaceColumn(column, df.Columns.[column].As<'T>() |> Series.mapAll f)

        df1        

    let trace (df: Frame<_, _>) =

        let df1 = df |> Frame.take 1
        df1.Print(true)

        df

    let getOptionalDate (column: string) (row: ObjectSeries<string>) =
        row.TryGetAs<DateTime> column
        |> OptionalValue.asOption
