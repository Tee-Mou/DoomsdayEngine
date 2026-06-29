#include <vector>
#include "Move.h"
#include "MoveTables.h"
#include "Game.h"

#pragma once
namespace Engine {
    
    #define LWCASTLE 4ULL * ((castleRights & 0b0001) != 0) * ((allBB & longMask) == 0)
    #define SWCASTLE 64ULL * ((castleRights & 0b0010) != 0) * ((allBB & shortMask) == 0)
    #define LBCASTLE 288230376151711744ULL * ((castleRights & 0b0100) != 0) * ((allBB & longMask) == 0)
    #define SBCASTLE 4611686018427387904ULL * ((castleRights & 0b1000) != 0) * ((allBB & shortMask) == 0)

    class Eval 
    {
        public:
            Eval(Game* game);
            std::vector<Move*> findLegalMoves();
            std::vector<Move*> findPieceMoves(Piece piece);
            u64 perft(int depth, bool recursion = false);
            std::vector<Move*> addCastleMoves(int piece);

            float evalPosition();
            float evalAlphaBeta(int depth, float alpha = -INFINITY, float beta=INFINITY);

            void makeMove(Move move);
            void unmakeMove(Move move);
            bool isCheck(Move move, bool self);
            bool checkMoveIsLegal(Move move);
            bool checksOpponent(Move move);
            void updateBitboards(Move move, bool unmake = false);

            // getters
            Game* getBoard() { return mGame; }
            void undoCastleRights();
            void undoEPFiles();
            void undoFiftyClock();

            // setters
            void setFen(str fen) { mGame->setFen(fen); }
            void addCastleToHistory(int rights) { mCastleHistory.push_back(rights); };
            void addEPToHistory(int epFiles) { mEPHistory.push_back(epFiles); };
            void addFiftyClockToHistory(int fiftyMoveClock) { mFiftyClockHistory.push_back(fiftyMoveClock); };

        private:
            Game* mGame;
            std::vector<int> mCastleHistory;
            std::vector<int> mEPHistory;
            std::vector<int> mFiftyClockHistory;
    };

} // namespace Engine