import "./Row.css"
import { Square } from './Square';

interface RowProps {
    rankNumber: number;
    rowPieces: number[];
}

function Row(props: RowProps) {
    
    return (
        <div className="Row">
            <Square rankNumber={props.rankNumber}
                    fileNumber={1}
                    piece={props.rowPieces[0]}/>
            <Square rankNumber={props.rankNumber}
                    fileNumber={2}
                    piece={props.rowPieces[1]}/>
            <Square rankNumber={props.rankNumber}
                    fileNumber={3}
                    piece={props.rowPieces[2]}/>
            <Square rankNumber={props.rankNumber}
                    fileNumber={4}
                    piece={props.rowPieces[3]}/>
            <Square rankNumber={props.rankNumber}
                    fileNumber={5}
                    piece={props.rowPieces[4]}/>
            <Square rankNumber={props.rankNumber}
                   fileNumber={6}
                    piece={props.rowPieces[5]}/>
            <Square rankNumber={props.rankNumber}
                    fileNumber={7}
                    piece={props.rowPieces[6]}/>
            <Square rankNumber={props.rankNumber}
                    fileNumber={8}
                    piece={props.rowPieces[7]}/>
        </div>
    )
}

export default Row