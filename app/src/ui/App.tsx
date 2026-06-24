import { useState } from 'react';
import reactLogo from './assets/react.svg';
import viteLogo from './assets/vite.svg';
import heroImg from './assets/hero.png';
import './App.css';
import Board from './Board';
import { FenToArray } from "./utils"

function App() {
  let currentFen: string = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  let pieceids: number[][] = FenToArray(currentFen);
  return (
    <>
      <div id="screen-container">
        <div className="engine-ui">
          <Board pieceIDs={pieceids}/>
          <div id="tools">
            <form id="fen-tools">
                <input id="fen-text" type="text" pattern="[a-zA-Z0-9/]+"></input>
                <input id="fen-button" type="button"></input>
            </form>
            <form id="analysis-tools">
                <input id="analysis-depth-input" type="text" pattern="[a-zA-Z0-9/]+"></input>
                <input id="analyse-button" type="button"></input>
                <output id="analysis-score">SCORE:</output>
                <output id="analysis-line">BEST LINE:</output>
            </form>
            <form id="perft-tools">
                <input id="perft-depth-input" type="text" pattern="[a-zA-Z0-9/]+"></input>
                <input id="perft-button" type="button"></input>
            </form>
          </div>
        </div>
      </div>
    </>
  )
}

export default App
