import { useState, type SetStateAction } from 'react';
import './App.css';
import Board from './Board';
import Tools from './Tools'
import { FenToArray } from "./utils"

function App() {
  const [currentFen, setCurrentFen]: [string, React.Dispatch<SetStateAction<string>>] = useState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  let pieceids: number[][] = FenToArray(currentFen);
  return (
    <>
      <div id="screen-container">
        <div className="engine-ui">
          <Board pieceIDs={pieceids}/>
          <Tools currentFen={currentFen}
                 setCurrentFen={setCurrentFen}/>
        </div>
      </div>
    </>
  )
}

export default App
