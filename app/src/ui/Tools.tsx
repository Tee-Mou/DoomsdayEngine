import { type SetStateAction, useEffect } from 'react';
import "./Tools.css"
import Console from "./Console"
import { log } from './utils';

interface ToolsProps {
    currentFen: string;
    setCurrentFen: React.Dispatch<SetStateAction<string>>;
    consoleOutput: string;
    setConsoleOutput: React.Dispatch<SetStateAction<string>>;
}

export function Tools (props: ToolsProps) {
    const { setCurrentFen, consoleOutput, setConsoleOutput } = props;

    useEffect(() => {
        try {   
            window.electronAPI.onResponse((line: string) => {
                // log(("Got Response:" + line), 2, consoleOutput, setConsoleOutput)
                // log(line, 2, consoleOutput, setConsoleOutput)
                if (line.substring(0, 8) === "RESULT: ") {
                    log("Got a result:", 2, consoleOutput, setConsoleOutput)
                    setConsoleOutput(prev => prev + line.substring(8) + '\n');
                    setCurrentFen(line.substring(8));
                }
                setConsoleOutput(prev => prev + line + '\n');
            })
        }
        catch {
            setConsoleOutput(prev => prev + "undefined" + '\n');
        }
    }, [setConsoleOutput]);

    const sendCommand = (command: string) => {
        if (!command.trim()) return;
        setConsoleOutput(prev => prev + `# ${command}\n`);
        window.electronAPI.sendCommand(command);
    };

    function handleFENForm (event: React.SubmitEvent<HTMLFormElement>) {
        event.preventDefault();
        const fen = event.target.fen.value;
        // props.setCurrentFen(fen);
        sendCommand(`fen ${fen}`);
    }

    function handlePerftForm (event: React.SubmitEvent<HTMLFormElement>) {
        event.preventDefault();
        sendCommand(`perft ${event.target.aDepth.value}`);
    }

    function handleAnalyseForm (event: React.SubmitEvent<HTMLFormElement>) {
        event.preventDefault();
        sendCommand(`analyse ${event.target.pDepth.value}`);
    }
    
    return (
        <div id="tools">
            <form id="fen-tools"
                  onSubmit={handleFENForm}>
                <input id="fen-text" type="text" name="fen" pattern="[a-zA-Z0-9/]+" placeholder="Enter FEN..."></input>
                <input id="fen-button" type="submit" value="< Submit >"></input>
            </form>
            <form id="analysis-tools"
                  onSubmit={handleAnalyseForm}>
                <input id="analysis-depth-input" type="text" name="aDepth" pattern="[a-zA-Z0-9/]+" placeholder='Enter Depth...'></input>
                <input id="analyse-button" type="submit" value="< ANALYSE >"></input>
                <output id="analysis-score">SCORE:</output>
                <output id="analysis-line">BEST LINE:</output>
            </form>
            <form id="perft-tools"
                  onSubmit={handlePerftForm}>
                <input id="perft-depth-input" type="text" name = "pDepth" pattern="[a-zA-Z0-9/]+" placeholder='Enter Depth...'></input>
                <input id="perft-button" type="submit" value="< PERFT >"></input>
            </form>
            <Console consoleOutput={props.consoleOutput} setConsoleOutput={props.setConsoleOutput}>
            </Console>
        </div>
    )
}

export default Tools