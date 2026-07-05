import { useState, useEffect, type SetStateAction } from 'react';
import './App.css';
import Board from './Board';
import Tools from './Tools'
import { FenToArray, log } from "./utils"
import { FenParseError } from './errors';

function App() {
  const [currentFen, setCurrentFen]: [string, React.Dispatch<SetStateAction<string>>] = useState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  const [consoleOutput, setConsoleOutput]: [string, React.Dispatch<SetStateAction<string>>] = useState("");

  let errorPieces: number[][] = [
      [0, 12, 12, 12, 12, 12, 12, 6],
      [12, 0, 12, 12, 12, 12, 6, 12],
      [12, 12, 0, 12, 12, 6, 12, 12],
      [12, 12, 12, 0, 6, 12, 12, 12],
      [12, 12, 12, 6, 0, 12, 12, 12],
      [12, 12, 6, 12, 12, 0, 12, 12],
      [12, 6, 12, 12, 12, 12, 0, 12],
      [6, 12, 12, 12, 12, 12, 12, 0],
  ];
  let pieceids: number[][] | FenParseError = FenToArray(currentFen);
  const displayPieces: number[][] = pieceids instanceof FenParseError ? errorPieces : pieceids;

  useEffect(() => {
    const parsed = FenToArray(currentFen);
    if (parsed instanceof FenParseError) {
      log(parsed.message, 0, consoleOutput, setConsoleOutput);
    } else {
      log("FEN string parsed successfully.", 2, consoleOutput, setConsoleOutput);
    }
  }, [currentFen]);


  return (
    <>
      <div id="screen-container">
        <div className="engine-ui">
          <Board pieceIDs={displayPieces}/>
          <Tools currentFen={currentFen}
                 setCurrentFen={setCurrentFen}
                 consoleOutput={consoleOutput}
                 setConsoleOutput={setConsoleOutput}/>
        </div>
      </div>
    </>
  )
}

export default App
