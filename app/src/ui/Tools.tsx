import { useState, type SetStateAction, useRef } from 'react';
import "./Tools.css"

interface ToolsProps {
    currentFen: string;
    setCurrentFen: React.Dispatch<SetStateAction<string>>;
}

export function Tools (props: ToolsProps) {
    const fenRef = useRef("");
    const analysisDepthRef = useRef(0);
    const perftDepthRef = useRef(0);

    function handleFENForm (event: React.SubmitEvent<HTMLFormElement>) {
        event.preventDefault();
        props.setCurrentFen(event.target.fen.value);
    }
    
    return (
        <div id="tools">
            <form id="fen-tools"
                  onSubmit={handleFENForm}>
                <input id="fen-text" type="text" name="fen" pattern="[a-zA-Z0-9/]+"></input>
                <input id="fen-button" type="submit"></input>
            </form>
            <form id="analysis-tools">
                <input id="analysis-depth-input" type="text" name="aDepth" pattern="[a-zA-Z0-9/]+"></input>
                <input id="analyse-button" type="submit"></input>
                <output id="analysis-score">SCORE:</output>
                <output id="analysis-line">BEST LINE:</output>
            </form>
            <form id="perft-tools">
                <input id="perft-depth-input" type="text" name = "pDepth" pattern="[a-zA-Z0-9/]+"></input>
                <input id="perft-button" type="submit"></input>
            </form>
        </div>
    )
}

export default Tools