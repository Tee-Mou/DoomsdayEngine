import "./Board.css";
import Row from "./Row";

interface BoardProps {
    pieceIDs: number[][]
}

function Board(props: BoardProps) {
    return (
        <div id="board">
            <Row rankNumber={8}
                 rowPieces={props.pieceIDs[7]}/>
            <Row rankNumber={7}
                 rowPieces={props.pieceIDs[6]}/>
            <Row rankNumber={6}
                 rowPieces={props.pieceIDs[5]}/>
            <Row rankNumber={5}
                 rowPieces={props.pieceIDs[4]}/>
            <Row rankNumber={4}
                 rowPieces={props.pieceIDs[3]}/>
            <Row rankNumber={3}
                 rowPieces={props.pieceIDs[2]}/>
            <Row rankNumber={2}
                 rowPieces={props.pieceIDs[1]}/>
            <Row rankNumber={1}
                 rowPieces={props.pieceIDs[0]}/>
        </div>
    )
} 
export default Board