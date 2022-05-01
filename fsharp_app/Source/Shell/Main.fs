namespace Shell

module Main =

    open Argu
    open Application.Types

    module A = Argument

    let printVersion () =
        let version = "v0.1"
        printfn $"%A{version}"

    let (|Config|_|) (arguments: ParseResults<A.CliArguments>) =
        if arguments.Contains A.Config then
            Some(arguments.GetResult A.Config)
        else
            None

    let (|UseCase|_|) (arguments: ParseResults<A.CliArguments>) =
        if arguments.Contains A.Use_Case then
            Some(arguments.GetResult A.Use_Case)
        else
            None

    let loadConfig (path: string) : Config =

        let config = Config()

        config.Load path

        config

    [<EntryPoint>]
    let main (argv: string array) =

        let arguments = A.parser.Parse argv
        let results = arguments.GetAllResults

        match arguments with
        | Config configPath & UseCase useCase ->
            let c = loadConfig configPath

            match useCase with
            | _ -> printfn "Not implemented yet."
        | _ -> A.printUsage A.parser
        0
