// Learn more about F# at http://fsharp.net
// See the 'F# Tutorial' project for more help.

open System
open System.Text
open System.IO
open System.Collections.Generic

open IndexLoader
open ProcessFunctions
open RequestManager

[<EntryPoint>]
let main argv = 
    let length = argv.Length
    if length <> 1 then
        let msg = "First argument: path to index"
        raise (System.Exception(msg))

    let indexPath = argv.[0]
    let indexTable = loadIndex indexPath
    printfn "%A" indexTable

    let mutable request = ""
    while (request <> ":q") do
        request <- Console.ReadLine()
        if (request <> ":q") then
            proceedRequest request indexTable
    0
