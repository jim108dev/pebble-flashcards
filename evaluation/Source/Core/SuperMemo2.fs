namespace Domain

module SuperMemo2 =

    open FSharpPlus.Data

    open Types

    let getLastStreak (xs: List<Answer>) =
        let rec go arr acc =
            match arr with
            | x :: t when x >= Answer.Correct -> go t acc + 1
            | _ -> acc

        go (xs |> List.rev) 0

    let a = 6.0
    let b = -0.8
    let c = 0.28
    let d = 0.02

    let min_score = 1.3
    let assumed_score = 2.0
    //theta When larger, the delays for correct answers will increase.
    let theta = 0.2

    /// Returns the number of days until seeing a problem again based on the
    /// history of answers to the problem
    /// https://gist.github.com/doctorpangloss/13ab29abd087dc1927475e560f876797#file-supermemo_2-py
    let getWaitingPeriod (answers: NonEmptyList<Answer>) :float =
        let easiness x = b + c * x + d * x * x

        let xs = answers |> NonEmptyList.toList

        if xs |> List.last < Answer.Correct then
            0.0
        else
            let streak = getLastStreak xs |> float

            let actualScore =
                xs |> List.map float |> List.sumBy easiness

            a * (max min_score (assumed_score + actualScore)) ** (theta * streak)
