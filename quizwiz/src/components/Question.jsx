import React from "react"
import he from "he"

export default function Question(props) {
    
    const [allOptions, setAllOptions] = React.useState([]);
    const [selectedOptionIndex, setSelectedOptionIndex] = React.useState(); // index will never be 4 so we can just default it to this
    
    function classAdder(option, index) {
        let classes = index === selectedOptionIndex ? "selectedOption" : "option"
        if (props.submitted) {
            if (option === props.answer) {
                classes += " correct"
            } else if (classes === "selectedOption"){
                classes += " wrong"
            }
        }
        return classes;
    }
    
    React.useEffect(() => {
        let shuffledArray = [props.answer]
        props.wrongAnswers.forEach((element) => {
            Math.floor(Math.random() * 2) == 1 
            ? shuffledArray.push(he.decode(element)) 
            : shuffledArray.unshift(he.decode(element))
            // 50/50 on whether it gets placed at the front or back of the array depending on if math.random generates a 1 or 0
        })
        setAllOptions(shuffledArray);
        setSelectedOptionIndex(null) // unselecting choice incase there is one from previous quiz
        }
    ,[props.answer])
    
    return (
        <div className="question">
            <h2>{props.question}</h2>
            
            <div className="options">
                {allOptions.map((option, index) => (
                    <label 
                        key={option} 
                        className={classAdder(option, index)}
                    >
                        {option}
                        <input 
                            name="answer" 
                            type="radio"
                            value={option}
                            checked={index === selectedOptionIndex}
                            onChange={() => {
                                if (!props.submitted) {
                                    setSelectedOptionIndex(index);
                                    props.setChoices(prevChoices => {
                                        const newChoices = [...prevChoices];
                                        newChoices[props.choicesIndex] = option;
                                        return newChoices;
                                    })
                                }
     
                            }}
                        />
                    </label>
                ))}
            </div>
            <hr/>
        </div>
    )
}