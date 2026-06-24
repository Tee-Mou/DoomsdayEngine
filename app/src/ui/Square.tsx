import "./Square.css"

// Piece icons in unicode. White piees in first then black pieces, ordered by descending value.
let pieces: string[] = ["♔" ,"♕" ,"♖" ,"♗" ,"♘" ,"♙" ,"♚" ,"♛" ,"♜" ,"♝" ,"♞" ,"♟", " "]

interface SquareProps {
    rankNumber: number;
    fileNumber: number;
    piece: number;
}

export function Square(props: SquareProps) {
    let fileLetter: string = String.fromCharCode(props.fileNumber + 96)

    let squareColour: string = (
        props.rankNumber + props.fileNumber
    ) % 2 === 1 ? "LightSquare" : "DarkSquare";

    let pieceStr: string = pieces[props.piece]

    return (
        <div className="Square">
            <div className={squareColour}>
                <div className="LocationContainer">
                    {fileLetter}{props.rankNumber}
                </div>
                <div className="PieceContainer">
                    {pieceStr}
                </div>
            </div>
        </div>
    )
}