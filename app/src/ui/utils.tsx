import { FenParseError } from "./errors";

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
    let emptyPieces: number[][] = [
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
    let endOfRank = false;
    try {
        while (true) {
            let pieceIdx: number = FenLookup[fen[idx]];
            let charCode: number = fen.charCodeAt(idx);
            
            // If character is ' ' or the string is over, we 
            // have reached the end of piece description.
            if (charCode === 32 || idx === n) {
                break;
            }

            // If character is '/', decrement the row ID.
            else if (endOfRank) {
                if (charCode === 47) {
                    rowIdx -= 1;
                    colIdx = 0;
                    endOfRank = false;
                } else throw new FenParseError("Could not Parse the provided FEN string: Expected '/'")
            }

            // Add to pieceRows if the character is a piece.
            else if (pieceIdx != null) {
                pieceRows[rowIdx][colIdx] = pieceIdx;
                if (colIdx === 7) endOfRank = true;
                colIdx += 1;
            }
    
            // Increment square if fen character is a number.
            else if (49 <= charCode && charCode <= 56) {
                colIdx = colIdx + (charCode - 48);
                if (colIdx > 8) {
                    throw new FenParseError("Could not Parse the provided FEN string: Number '" + fen[idx] + "' is too large.");
                } else if (colIdx === 8) endOfRank = true;
            }
    
            else throw new FenParseError("Could not Parse the provided FEN string: Unpexpected symbol: '" + fen[idx] + "'.")
            idx += 1;
        }
    } catch (err) {
        if (err instanceof FenParseError) {
            console.error(err.message);
            return emptyPieces;
        } else {
            throw err;
        }
    }

    return pieceRows;
}