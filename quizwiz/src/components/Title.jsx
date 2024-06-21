import React from "react"

export default function Title(props) {
    return (
        <div className="titleScreen">
            <h1>QuizWiz</h1>
            <h3>A simple general knowledge quiz</h3>
            <button onClick={props.handleClick}>Start Quiz</button>
        </div>
    )
}