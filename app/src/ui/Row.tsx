import "./Row.css"

interface RowProps {
    rankNumber: number;
}

interface SquareProps {
    rankNumber: number;
    fileNumber: number;
}

function Square(props: SquareProps) {
    let fileLetter: string = String.fromCharCode(props.fileNumber + 96)

    let squareColour: string = (
        props.rankNumber + props.fileNumber
    ) % 2 === 1 ? "LightSquare" : "DarkSquare";

    return (
        <div className="Square">
            <div className={squareColour}>{fileLetter}{props.rankNumber}</div>
        </div>
    )
}

function Row(props: RowProps) {
    return (
        <div className="Row">
            <Square rankNumber={props.rankNumber}
                    fileNumber={1}/>
            <Square rankNumber={props.rankNumber}
                fileNumber={2}/>
            <Square rankNumber={props.rankNumber}
                fileNumber={3}/>
            <Square rankNumber={props.rankNumber}
                fileNumber={4}/>
            <Square rankNumber={props.rankNumber}
                fileNumber={5}/>
            <Square rankNumber={props.rankNumber}
                fileNumber={6}/>
            <Square rankNumber={props.rankNumber}
                fileNumber={7}/>
            <Square rankNumber={props.rankNumber}
                fileNumber={8}/>
        </div>
    )
}

export default Row