namespace Application

module Storage =

    open System.IO
    open Deedle

    [<Literal>]
    let separator = ';'

    let read (path: string) =
        use reader = new StreamReader(path, new System.Text.UTF8Encoding(true))
        Frame.ReadCsv(
            reader,
            separators = separator.ToString(),
            hasHeaders = true,
            inferTypes = true
        )

    let write (path: string) (df: Frame<_, _>) =
        let df1 = df |> Frame.indexRowsOrdinally
        use writer = new StreamWriter(path, false, new System.Text.UTF8Encoding(true))
        df1.SaveCsv(
            writer,
            includeRowKeys = false,
            separator = separator
        )

    let writeVerbose (path: string) (df: Frame<_, _>) =
        write path df
        printfn $"Written {df.RowCount} rows into {path}."

    let copy (dest: string) (source: string) =
        File.Copy(dest, source, true)