module IndexLoader

open System
open System.Text
open System.IO
open System.Collections.Generic

type System.String with
    member x.Split(separator : (string [])) = x.Split(separator, System.StringSplitOptions.None)

let loadIndex (indexPath : string) = 
    printfn "Loading index..."    
    let indexFile = FileInfo(indexPath)

    if not <| indexFile.Exists then
        let msg = System.String.Format("Index file not found: {0}", indexPath)
        raise (System.IO.FileNotFoundException(msg))

    let readLines (filePath : string) = seq {
        use sr = new StreamReader (filePath, System.Text.Encoding.UTF8)
        while not sr.EndOfStream do
            yield sr.ReadLine ()
    }

    let fileContentInLines = readLines indexPath
    let indexTable = new Dictionary<string, string>()
    
    let parseFileContent (fileContentInLines : seq<string>) = 
        let mutable fileContentInWords = []
        let separator = [|"@"|]
        for x in fileContentInLines do
            let splittedPath = x.Split separator
            indexTable.Add(splittedPath.[0], splittedPath.[1])

    parseFileContent fileContentInLines
    printfn "Index loaded!"
    indexTable
