module Stemmer

open System
open System.IO
open System.Text

type System.String with
    member x.Split(separator : (string [])) = x.Split(separator, System.StringSplitOptions.None)

let exec processName args =
    let psi = new System.Diagnostics.ProcessStartInfo(processName)
    psi.Arguments <- args
    psi.UseShellExecute <- false
    let p = System.Diagnostics.Process.Start(psi)
    p.WaitForExit()
    p.ExitCode

let formStemmedFileName (fileName : string) = 
    let separator = [|"."|]
    let splittedName = fileName.Split separator

    let fileNameWithoutExtension = splittedName.[0]
    let stemmedFileName = fileNameWithoutExtension + "_stemmed.txt"
    stemmedFileName

let formStemmedFilePath (inputFilesDir : string) =
    let separator = [|"/"|]
    let splittedPath = inputFilesDir.Split separator
    let length = splittedPath.Length

    let rootPath = splittedPath.[0..length - 2]
    let stemmedPath = String.Join("/", rootPath) + "/stemmed"

    stemmedPath

let createStemmedDir (stemmedPath : string) = 
    let stemmedDir = new System.IO.DirectoryInfo(stemmedPath)

    if System.IO.Directory.Exists(stemmedPath) then
        for file in stemmedDir.GetFiles() do
            file.Delete()
    else
        stemmedDir.Create()
    
let stem (file : FileInfo) (pathToMyStem : string) (stemmedPath : string)= 
    let fileName = file.Name
    let fullFileName = file.FullName

    let stemmedFileName = formStemmedFileName fileName
    let stemmedFilePath = stemmedPath + "/" + stemmedFileName;

    exec pathToMyStem (fullFileName + " " + stemmedFilePath)

let stemDirectory (path : string) (pathToStem : string) = 
    let documentsDir = new System.IO.DirectoryInfo(path)

    if not <| System.IO.Directory.Exists(path) then
        let msg = System.String.Format("Source directory does not exist: {0}", path)
        raise (System.IO.DirectoryNotFoundException(msg))

    let stemmedFilesPath = formStemmedFilePath path
    createStemmedDir stemmedFilesPath

    for file in documentsDir.GetFiles() do
        stem file pathToStem stemmedFilesPath |> ignore
    stemmedFilesPath