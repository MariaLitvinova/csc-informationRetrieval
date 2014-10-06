module RequestManager

open System
open System.Text
open System.IO
open System.Collections.Generic

type System.String with
    member x.Split(separator : (string [])) = x.Split(separator, System.StringSplitOptions.None)

let splitIndexValue (indexValue : string) = 
    let answerSeparator = [|" "|]
    let splittedAnswer = indexValue.Split answerSeparator
    splittedAnswer

let outputList (answer : List<string>) = 
    if answer.Count = 0 then
        printfn "Nothing found"
    else
        printf "Found: "
        for word in answer do
            printf "%A " word
        printf "\n"

let proceedOrRequest (request : string) (indexTable : Dictionary<string, string>) = 
    let separator = [|"OR"|]
    let splittedRequest = request.Split separator

    let mutable answer = new List<string>()
    for word in splittedRequest do
        if indexTable.ContainsKey word then
            let splittedAnswer = splitIndexValue (indexTable.Item(word))

            for simpleAnswer in splittedAnswer do
                if not <| answer.Contains(simpleAnswer) then
                    answer.Add(simpleAnswer)
    
    outputList answer

let proceedAndRequest (request : string) (indexTable : Dictionary<string, string>) = 
    let separator = [|"AND"|]
    let splittedRequest = request.Split separator

    let mutable answer = List<string>()
    let mutable firstWord = true

    for word in splittedRequest do
        if indexTable.ContainsKey word then
            let splittedAnswer = splitIndexValue (indexTable.Item(word))
            if firstWord then
                for simpleAnswer in splittedAnswer do
                    answer.Add(simpleAnswer)
            else
                let filteredAnswer = Seq.filter(fun elem -> Array.IndexOf(splittedAnswer, elem) > -1) answer |> Seq.toList
                answer.Clear()
                for word in filteredAnswer do
                    answer.Add(word)

            if (splittedAnswer.Length > 0) then
                firstWord <- false
        else
            answer.Clear()
    
    outputList answer

let proceedSimpleRequest (request : string) (indexTable : Dictionary<string, string>) = 
    if indexTable.ContainsKey(request) then
        printfn "Found: %A" (indexTable.Item(request))
    else
        printfn "Nothing found"

let incorrectRequest (request : string) = 
    let tooManyOperators = request.Contains("OR") && (request.Contains("AND"))
    let separator = [|" "|]
    let numberOfWords = request.Split separator |> Array.length
    let tooManyWords = (not <| request.Contains("OR")) && (not <| request.Contains("AND")) && (numberOfWords > 1)
    tooManyOperators || tooManyWords

let proceedRequest (request : string) (indexTable : Dictionary<string, string>) = 
    if incorrectRequest request then
        printfn "Incorrect request"
    else 
        let requestWithoutSpaces = request.Replace(" ", "")
        if requestWithoutSpaces.Contains("OR") then
            proceedOrRequest requestWithoutSpaces indexTable
        else
            if requestWithoutSpaces.Contains("AND") then
                proceedAndRequest requestWithoutSpaces indexTable
            else
                proceedSimpleRequest requestWithoutSpaces indexTable 