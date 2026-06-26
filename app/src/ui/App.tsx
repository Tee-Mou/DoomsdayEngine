import { useState, type SetStateAction } from 'react';
import './App.css';
import Board from './Board';
import Tools from './Tools'
import { FenToArray, log } from "./utils"
import { FenParseError } from './errors';

function App() {
  const [currentFen, setCurrentFen]: [string, React.Dispatch<SetStateAction<string>>] = useState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  const [consoleOutput, setConsoleOutput]: [string, React.Dispatch<SetStateAction<string>>] = useState("");
  const [renderedLast, setRenderedLast]: [boolean, React.Dispatch<SetStateAction<boolean>>] = useState(false);

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

  if (pieceids instanceof FenParseError) {
    if (!renderedLast) {
      setRenderedLast(true);
      log(pieceids.message, 0, consoleOutput, setConsoleOutput);
    } 
    pieceids = errorPieces;
  } else if (!renderedLast) {
    setRenderedLast(true);
    log("FEN string parsed sucessfully.", 2, consoleOutput, setConsoleOutput);
  }

  return (
    <>
      <div id="screen-container">
        <div className="engine-ui">
          <Board pieceIDs={pieceids}/>
          <Tools currentFen={currentFen}
                 setCurrentFen={setCurrentFen}
                 consoleOutput={consoleOutput}
                 setConsoleOutput={setConsoleOutput}
                 renderedLastError={renderedLast}
                 setRenderedError={setRenderedLast}/>
        </div>
      </div>
    </>
  )
}

export default App
