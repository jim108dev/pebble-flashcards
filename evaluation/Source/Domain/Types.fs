namespace Domain

module Types =

    open System

    module C = 
        let id = "id"
        let feedbackHistory = "feedback_history"
        let startHistory = "start_history"
        let stopHistory = "stop_history"
        let buried = "buried"
        let latestBuried = "latest_buried"
        let feedback = "feedback"
        let start = "start"
        let stop = "stop"
        let waitingPeriod = "waitingPeriod"
        let text1 = "text1"
        let text2 = "text2"
        let source = "source"
        let tags = "tags"

    type Answer =
        | Incomprehensible = 0
        | IncorrectButComprehensible = 1
        | Correct = 2
        | Easy = 3
        | Buried = 4

    type Id = string
    type FeedbackHistory = string
    type StartHistory = string
    type StopHistory = string 
    type Buried = Boolean
    type Feedback = Answer
    type FeedbackIndex = int
    type Start = DateTime
    type Stop = DateTime

    type Period = float 
    type Days = int

