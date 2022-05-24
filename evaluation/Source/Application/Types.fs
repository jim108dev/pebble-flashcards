namespace Application

module Types =

    open FSharp.Configuration

    type Filename = string

    type Config = YamlConfig<"Config/Config.yaml">