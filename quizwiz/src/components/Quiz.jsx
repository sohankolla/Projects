import React from "react";
import he from "he";
import Question from "./Question.jsx";

export default function Quiz() {
    const [questionSet, setQuestionSet] = React.useState([]);
    const [choices, setChoices] = React.useState(Array(5).fill(null));
    const [submitted, setSubmitted] = React.useState(false);
    const [score, setScore] = React.useState(0);
    const [resetTriggered, setResetTriggered] = React.useState(false);
    const [loading, setLoading] = React.useState(true);

    React.useEffect(() => {
        async function getQuestions() {
            setLoading(true);
            try {
                let response = await fetch("https://opentdb.com/api.php?amount=5&category=9&difficulty=medium&type=multiple");
                let data = await response.json();
                while (data.response_code !== 0) { // response code 0 means successful
                    await new Promise(resolve => setTimeout(resolve, 1000));
                    response = await fetch("https://opentdb.com/api.php?amount=5&category=9&difficulty=medium&type=multiple");
                    data = await response.json();
                }
                setQuestionSet(data.results.map(question => ({
                    ...question,
                    correct_answer: he.decode(question.correct_answer),
                    question: he.decode(question.question),
                    incorrect_answers: question.incorrect_answers.map(he.decode)
                })));
            } catch (error) {
                console.error(error.name); 
            } finally {
                setLoading(false); // Turn loading screen off
            }
        }
        getQuestions();
    }, [resetTriggered]);

    function submitQuiz() {
        let newScore = 0;
        questionSet.forEach((question, index) => {
            if (choices[index] === question.correct_answer) {
                newScore += 1;
            }
        });
        setScore(newScore);
        setSubmitted(true);
    }

    function restartQuiz() {
        setScore(0);
        setSubmitted(false);
        setChoices(Array(5).fill(null));
        setResetTriggered(prev => !prev); // Toggle to re-fetch questions
    }

    if (loading) {
        return <div>Loading...</div>;
    }

    const questions = questionSet.map((question, index) => (
        <Question
            key={index}
            question={question.question}
            answer={question.correct_answer}
            wrongAnswers={question.incorrect_answers}
            choicesIndex={index}
            setChoices={setChoices}
            submitted={submitted}
        />
    ));

    return (
        <div className="quizScreen">
            {questions}
            {submitted ? (
                <div>
                    You scored {score}/5
                    <button onClick={restartQuiz} type="reset">Play Again</button>
                </div>
            ) : (
                <button onClick={submitQuiz} type="button">Check Answers</button>
            )}
        </div>
    );
}
