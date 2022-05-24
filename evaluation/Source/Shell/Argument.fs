namespace Shell

module Argument =

    open Argu
    open System

    type UseCase =      
        | Align_History = 1
        | Merge_Feedback = 2
        | Prepare_Next_Session = 3


    type CliArguments =
        | [<Mandatory;Unique;AltCommandLine("-c")>] Config of path:string
        | [<Unique;AltCommandLine("-u")>] Use_Case of UseCase

        interface IArgParserTemplate with
            member s.Usage =
                match s with
                | Config _ -> "specify a yaml config."
                | Use_Case _ -> "Specify a use case."

    let errorHandler =
        ProcessExiter
            (colorizer =
                function
                | ErrorCode.HelpText -> None
                | _ -> Some ConsoleColor.Red)

    let printUsage (parser: ArgumentParser) = printfn "%s" <| parser.PrintUsage()

    let parser =
        ArgumentParser.Create<CliArguments>(programName = "pebble-flashcards", errorHandler = errorHandler)
            
