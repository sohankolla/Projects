import React from "react"
import Quiz from "./components/Quiz.jsx"
import Title from "./components/Title.jsx"

export default function App() {
    
    const [quizScreen, setQuizScreen] = React.useState(false)
    
    function startQuiz() {
        setQuizScreen(true)
    }
    
    return (
        <main>
            {quizScreen ? <Quiz /> : <Title handleClick={startQuiz}/>}
        </main>
    )
}