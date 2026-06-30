#include <string>

#pragma once

using u64 = unsigned long long;
using str = std::string;

namespace Engine {

str static squares[64] = {
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
};

class Game 
{
    public:
        Game(str fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

        void processPieceLocations(str locFen);
        void processCastlingRights(str castleFen);
        void processEnPassantFiles(str epFen);
        void processCurrentTurn(str turnFen, str moveFen);
        void processFiftyMoveClock(str fiftyMoveFen);
        str calculateFEN();
        
        // getters
        str getFen() { return mFen; }
        u64* getBitboard(int index) { return &pieceLocations[index]; }
        int getCastles() { return castlingRights; }
        int getEPFiles() { return enPassantFiles; }
        int getHalfMove() { return halfMove; }
        int getFiftyMoveClock() { return fiftyClock; }
        bool getTurn() { return !(halfMove % 2); }
        
        //setters
        void setFen(str fen);
        void setCastlingRights(int rights) { castlingRights = rights; }
        void setEPFiles(int files) { enPassantFiles = files; }
        void incrementMove() { halfMove++; }
        void decrementMove() { halfMove--; }
        void incrementFiftyClock() { fiftyClock++; }
        void setFiftyClock(int clock) { fiftyClock = clock; }
        void resetFiftyClock() { fiftyClock = 0; }

    private:
        str mFen;
        u64 pieceLocations[15];
        int castlingRights;
        int enPassantFiles;
        int halfMove;
        int fiftyClock;
};

} // namespace Engine