import "./Board.css";
import Row from "./Row";

function Board() {
    return (
        <div id="board">
            <Row rankNumber={8}/>
            <Row rankNumber={7}/>
            <Row rankNumber={6}/>
            <Row rankNumber={5}/>
            <Row rankNumber={4}/>
            <Row rankNumber={3}/>
            <Row rankNumber={2}/>
            <Row rankNumber={1}/>
        </div>
    )
} 
export default Board