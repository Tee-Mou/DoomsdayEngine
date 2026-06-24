interface IHash {
    [pieceID : string] : number;
}

export function FenToArray (fen: string) : number[][] {
    /**
     * Converts a fen string into an array of arrays of piece IDs.
     * 
     * @param fen - The FEN string to be converted
     * @returns The array of arrays of piece IDs.
     */
    let FenLookup: IHash = {
        "K": 0, "Q": 1, "R": 2, "B": 3, "N":  4, "P":  5,
        "k": 6, "q": 7, "r": 8, "b": 9, "n": 10, "p": 11
    };

    // Row 0 is rank 1. Column 0 is file A.
    let pieceRows: number[][] = [
        [12, 12, 12, 12, 12, 12, 12, 12],
        [12, 12, 12, 12, 12, 12, 12, 12],
        [12, 12, 12, 12, 12, 12, 12, 12],
        [12, 12, 12, 12, 12, 12, 12, 12],
        [12, 12, 12, 12, 12, 12, 12, 12],
        [12, 12, 12, 12, 12, 12, 12, 12],
        [12, 12, 12, 12, 12, 12, 12, 12],
        [12, 12, 12, 12, 12, 12, 12, 12],
    ];
    let n: number = fen.length;
    let idx: number = 0;
    let rowIdx: number = 7;
    let colIdx: number = 0;
    while (idx < n) {
        let pieceIdx: number = FenLookup[fen[idx]];
        let charCode: number = fen.charCodeAt(idx);
        
        // Add to pieceRows if the character is a piece.
        if (pieceIdx != null) {
            pieceRows[rowIdx][colIdx] = pieceIdx;
            colIdx = (colIdx + 1) % 8;
        }

        // Increment square if fen character is a number.
        else if (49 <= charCode && charCode <= 56) {
            colIdx = (colIdx + charCode) % 8;
        }
        
        // If character is '/', decrement the row ID.
        else if (charCode === 47) {
            rowIdx -= 1;
        }

        // If character is ' ', we have reached the
        // end of piece description.
        else if (charCode === 32) {
            break;
        }
        idx += 1;
    }
    return pieceRows;
}