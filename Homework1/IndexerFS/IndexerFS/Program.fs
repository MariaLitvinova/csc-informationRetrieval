// Learn more about F# at http://fsharp.net
// See the 'F# Tutorial' project for more help.

open System
open System.IO
open System.Text
open System.Collections.Generic

open Stemmer

type System.String with
    member x.Split(separator : (string [])) = x.Split(separator, System.StringSplitOptions.None)

let formIndexDirectory (documentsToIndexPath : string) =
    let separator = [|"/"|]
    let splittedPath = documentsToIndexPath.Split separator
    let directoryLength = splittedPath.Length

    let rootPath = splittedPath.[0..directoryLength - 2]
    let indexPath = String.Join("/", rootPath) + "/index"

    indexPath

let createIndexDirectory (indexPath : string) = 
    let indexDir = new System.IO.DirectoryInfo(indexPath)

    if System.IO.Directory.Exists(indexPath) then
        for file in indexDir.GetFiles() do
            file.Delete()
    else
        indexDir.Create()

let proceedOneWord (word : string) (documentName : string) (indexTable : Dictionary<string, string list>) = 
    if not <| indexTable.ContainsKey(word) then
        indexTable.Add(word, [documentName])
    else
        let containsDocument document list = List.exists (fun elem -> elem = document) list
        if not <| containsDocument documentName (indexTable.Item(word)) then
            indexTable.Item(word) <- indexTable.Item(word) @ [documentName]
        

let proceedOneDocument (file : FileInfo) (indexTable : Dictionary<string, string list>) = 
    let filePath = file.FullName
    let documentName = file.Name

    printfn "proceeding : %A" filePath

    let readLines (filePath : string) = seq {
        use sr = new StreamReader (filePath, System.Text.Encoding.UTF8)
        while not sr.EndOfStream do
            yield sr.ReadLine ()
    }

    let fileContentInLines = readLines filePath
    
    let parseFileContent (fileContentInLines : seq<string>) = 
        let mutable fileContentInWords = []
        let separator = [|"{"; "}"; " "; "|"|]
        for x in fileContentInLines do
            let splittedPath = x.Split separator |> Array.toList
            fileContentInWords <- List.append fileContentInWords splittedPath
        fileContentInWords

    let fileContentInWords = parseFileContent fileContentInLines
    fileContentInWords |> List.iter (fun x -> proceedOneWord x documentName indexTable)

let indexForDocuments (documentsPath : string) =
    let documentsDir = new System.IO.DirectoryInfo(documentsPath)

    if not <| System.IO.Directory.Exists(documentsPath) then
        let msg = System.String.Format("Source directory does not exist: {0}", documentsPath)
        raise (System.IO.DirectoryNotFoundException(msg))

    let indexTable = new Dictionary<string, string list>()

    for file in documentsDir.GetFiles() do
        proceedOneDocument file indexTable

    indexTable

let saveToFile (indexTable : Dictionary<string, string list>) (indexPath : string) = 
    let indexFilePath = indexPath + "/index.txt"
    
    if File.Exists(indexFilePath) then
        File.Delete(indexFilePath)

    let indexFile = File.CreateText(indexFilePath)

    for record in indexTable do
        indexFile.Write(record.Key + "@")
        for v in record.Value do
            indexFile.Write(v)
            indexFile.Write(" ")
        indexFile.WriteLine()

    indexFile.Dispose()

[<EntryPoint>]
let main argv = 
    let length = argv.Length
    if length <> 2 then
        let msg = "First argument: path to documents, second argument: path to mystem.exe"
        raise (System.Exception(msg))

    let documentsPath = argv.[0]
    let stemmerPath = argv.[1]

    let stemmedFilesPath = stemDirectory documentsPath stemmerPath

    let indexPath = formIndexDirectory stemmedFilesPath
    createIndexDirectory indexPath

    let indexTable = indexForDocuments stemmedFilesPath
    saveToFile indexTable indexPath

    0
