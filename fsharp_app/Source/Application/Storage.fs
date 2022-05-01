namespace Application

module Storage =

    open System.IO
    open Deedle

    [<Literal>]
    let separator = ';'

    let read (localPath: string) =
        Frame.ReadCsv(
            localPath,
            separators = separator.ToString(),
            hasHeaders = true,
            inferTypes = true
        )

    let write (localPath: string) (df: Frame<_, _>) =
        let df1 = df |> Frame.indexRowsOrdinally
        use writer = new StreamWriter(localPath, false, new System.Text.UTF8Encoding(true))
        df1.SaveCsv(
            writer,
            includeRowKeys = false,
            separator = separator
        )

    let writeVerbose (localPath: string) (df: Frame<_, _>) =
        write localPath df
        printfn $"Written {df.RowCount} rows into {localPath}."

    let copy (dest: string) (source: string) =
        File.Copy(dest, source, true)