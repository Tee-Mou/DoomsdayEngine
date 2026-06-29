#include "../inc/Eval.h"
#include "../inc/Utils.h"
#include <cmath>

namespace Engine {

Eval::Eval(Game* game) : mGame(game) {
    addCastleToHistory(game->getCastles());
    addEPToHistory(game->getEPFiles());
    addFiftyClockToHistory(game->getFiftyMoveClock());

    MoveTables::generateEdgeMasks();
    MoveTables::generateBishopMasks();
    MoveTables::generateRookMasks();
    
    // for (int i = 0; i < 64; i++) {
    //     MoveTables::generateMagic(i, ROOK);
    //     MoveTables::generateMagic(i, BISHOP);
    // }
    
    MoveTables::generateKingAttacksTable();
    MoveTables::generatePawnTables();
    MoveTables::generateMagicAttacksTable(ROOK);
    MoveTables::generateMagicAttacksTable(BISHOP);
    MoveTables::generateKnightAttacksTable();
}

u64 Eval::perft(int depth, bool recursion) {
    std::vector<Move*> moves;
    u64 nodes = 0;
    u64 captures = 0;
    // std::cout << *getBoard()->getBitboard(4)<< std::endl;
    moves = findLegalMoves();
    int moveCount = moves.size();
    
    if (depth == 1) {
        // for (int i = 0; i < moveCount; i++) {
        //     { std::cout << "    " << moves[i]->oldSquare << "|" << moves[i]->newSquare << std::endl; }
        // }
        if (!recursion) {
            for (int i = 0; i < moveCount; i++) {
                // std::cout << squares[moves[i]->oldSquare] << squares[moves[i]->newSquare] << ": " << nodes << std::endl;
            }
        }
        return (u64)moveCount;
    }


    for (int i = 0; i < moveCount; i++) {
        u64 prevNodes = nodes;
        u64 prevCaps = captures;
        makeMove(*moves[i]);
        // std::cout << moves[i]->oldSquare << "|" << moves[i]->newSquare << std::endl;
        u64 next = perft(depth - 1, true);
        nodes += next;
        // if (!recursion) { std::cout << squares[moves[i]->oldSquare] << squares[moves[i]->newSquare] << ": " << nodes-prevNodes << std::endl; }
        unmakeMove(*moves[i]);
        // if (depth == 2) { std::cout << nodes - prevNodes << std::endl; }
    }
    return (u64)nodes;
}

float Eval::evalAlphaBeta(int depth, float alpha, float beta) {
    float eval;
    if (depth == 0) { return evalPosition(); };
    
    Game* board = getBoard();
    Colour turn = (Colour)board->getTurn();
    if (turn) {
        eval = -INFINITY;
        std::vector<Move*> moves = findLegalMoves();
        if (moves.size() == 0) { return evalPosition(); }
        std::vector<Move*>::iterator it;
        for (it = moves.begin(); it != moves.end(); ++it) {
            Move move = **it;
            makeMove(move);
            eval = evalAlphaBeta(depth--, alpha, beta);
            unmakeMove(move);
            if (eval > beta) {
                break;
            }
            else if (eval > alpha) {
                alpha = eval;
            }
        }
        return eval;
    }
    else {
        eval = INFINITY;
        std::vector<Move*> moves = findLegalMoves();
        if (moves.size() == 0) { return evalPosition(); }
        std::vector<Move*>::iterator it;
        for (it = moves.begin(); it != moves.end(); ++it) {
            Move move = **it;
            makeMove(move);
            eval = evalAlphaBeta(depth - 1, alpha, beta);
            unmakeMove(move);
            if (eval < alpha) {
                break;
            }
            else if (eval < beta) {
                beta = eval;
            }
        }
        return eval;
    }
}

float Eval::evalPosition() {
    int pieceVals[6] = {0, 1, 3, 3, 5, 9};
    int player[2] = {-1, 1};
    float eval = 0;
    
    Game* board = getBoard();
    for (int i = 0; i < 13; i++) {
        u64 pieceBB = *board->getBitboard(i);
        int count = Utils::csb(pieceBB);
        eval += pieceVals[i % 6] * player[i < 6] * count;
    }
    return eval;
}

std::vector<Move*> Eval::findPieceMoves(Piece piece) {
    std::vector<Move*> moves;
    Game* board = getBoard();
    Colour turn = (Colour)board->getTurn();

    int pieceIndex = piece + !turn * 6;
    u64 pieceBB = *board->getBitboard(pieceIndex);
    u64 friendlyBB = *board->getBitboard(turn ? 12 : 13);
    u64 enemyBB = *board->getBitboard(turn ? 13 : 12);
    u64 allBB = *board->getBitboard(14);
    u64 epBB = (u64)board->getEPFiles() << (turn ? 40 : 16);


    while (pieceBB > 0) {
        int sq = Utils::ctz(pieceBB);
        u64 sqBB = 1ULL << sq;

        u64 moveBB;
        std::vector<Move*> castleMoves;
        u64 mask;
        u64 blockers;

        switch (piece) {
            case KING:
                moveBB = MoveTables::getKingMoves(sq) & ~friendlyBB;
                // std::cout << "KING BB: " << moveBB << std::endl;
                castleMoves = addCastleMoves(pieceIndex);
                moves.insert(moves.end(), castleMoves.begin(), castleMoves.end());
                break;
            case PAWN:
                enemyBB |= epBB;
                moveBB = MoveTables::getPawnMoves(sq, turn) & ~allBB;
                if (!(moveBB & (turn ? sqBB << 8 : sqBB >> 8))) { moveBB = 0; }
                moveBB |= MoveTables::getPawnAttacks(sq, turn) & (enemyBB | epBB);
                break;
            case BISHOP: case ROOK:
                mask = MoveTables::getMask(sq, piece);
                blockers = mask & allBB;
                moveBB = MoveTables::getMagicMoves(sq, piece, blockers) & ~friendlyBB;
                // std::cout << (piece == ROOK ? "ROOKBB: " : "BISHOPBB: ") << moveBB << std::endl;
                // if (piece == ROOK && sq == 0 && (blockers & 16777218)) {
                //     std::cout << "Required output: " << MoveTables::getMagicMoves(sq, piece, 0) << std::endl; 
                //     std::cout << "blockers for a1 rook: " << blockers << std::endl;
                //     std::cout << "moves for a1 rook: " << moveBB << std::endl;
                //     getchar();
                // }
                break;
            case KNIGHT:
                moveBB = MoveTables::getKnightMoves(sq) & ~friendlyBB;
                break;
            case QUEEN:
                u64 bishopMask = MoveTables::getMask(sq, BISHOP);
                u64 rookMask = MoveTables::getMask(sq, ROOK);
                u64 bishopBlockers = bishopMask & allBB;
                u64 rookBlockers = rookMask & allBB;
                moveBB = MoveTables::getMagicMoves(sq, BISHOP, bishopBlockers);
                moveBB |= MoveTables::getMagicMoves(sq, ROOK, rookBlockers);
                moveBB &= ~friendlyBB;
                break;
        }
        while (moveBB > 0) {
            int newSq = Utils::ctz(moveBB);
            u64 newSqBB = 1ULL << newSq;
            int cPiece = -1;
            if (newSqBB & enemyBB) {
                if (piece == PAWN && (newSqBB & epBB)) { cPiece = 1 + 6 * turn; }
                else {
                    for (int i = 0; i < 6; i++) {
                        if (*board->getBitboard(i + 6 * turn) & newSqBB) { cPiece = i + 6 * turn; };
                    }
                };
            }
            if (piece == PAWN && ((newSq > 55) & turn) || (newSq < 8) & !turn) {
                for (int i = 2; i < 6; i++) {
                    int pPiece = i + !turn * 6;
                    Move* move = new Move(sq, newSq, pieceIndex, cPiece, pPiece); 
                    bool legal = checkMoveIsLegal(*move);
                    if (legal) { moves.push_back(move); }
                }
            }
            else { 
                Move* move = new Move(sq, newSq, pieceIndex, cPiece); 
                bool legal = checkMoveIsLegal(*move);
                if (legal) { moves.push_back(move); }
            }
            moveBB ^= newSqBB;
        }
        pieceBB ^= sqBB;
    }
    return moves;
};

std::vector<Move*> Eval::findLegalMoves() {
    std::vector<Move*> legalMoves;
    for (int i = 0; i < 6; i++) {
        std::vector<Move*> moves = findPieceMoves((Piece)i);
        legalMoves.insert(legalMoves.end(), moves.begin(), moves.end());
    }
    return legalMoves;
}

void Eval::makeMove(Move move) {
    Game* board = getBoard();
    updateBitboards(move);

    if (move.piece == 0 || move.piece == 6) {
        int rights = board->getCastles();
        rights &= 3 << (2 - (move.piece / 3));
        board->setCastlingRights(rights);
    }

    if (move.piece == 4 || move.piece == 10) {
        int currentRights = board->getCastles();
        if (move.oldSquare == 0 || move.oldSquare == 7 || move.oldSquare == 56 || move.oldSquare == 63) {
            int lostCastle = 1 << (move.oldSquare % 6); 
            board->setCastlingRights(currentRights & ~lostCastle);
        }
    }

    int newEPFiles = 0;
    if ((move.piece == 1 || move.piece == 7)){
        board->resetFiftyClock();
        if (abs(move.oldSquare - move.newSquare) == 16) {
            int file = move.oldSquare % 8;
            newEPFiles = 1 << file;
        }
    }
    if (move.cPiece != -1) { board->resetFiftyClock(); }

    board->setEPFiles(newEPFiles);

    addCastleToHistory(board->getCastles());
    addEPToHistory(board->getEPFiles());
    addFiftyClockToHistory(board->getFiftyMoveClock());
    board->incrementMove();
}

void Eval::unmakeMove(Move move) {
    Game* board = getBoard();
    board->decrementMove();
    updateBitboards(move, true);
    undoEPFiles();
    undoCastleRights();
    undoFiftyClock();
}

bool Eval::checkMoveIsLegal(Move move) {
    updateBitboards(move);
    Game* board = getBoard();
    bool turn = move.piece < 6;

    u64 kingBB = *board->getBitboard(6 * (!turn));
    int kingSq = Utils::ctz(kingBB);
    u64 enemyKing = *board->getBitboard(6 * turn);
    u64 enemyPawns = *board->getBitboard(6 * turn + 1);
    u64 enemyBishops = *board->getBitboard(6 * turn + 2);
    u64 enemyKnights = *board->getBitboard(6 * turn + 3);
    u64 enemyRooks = *board->getBitboard(6 * turn + 4);
    u64 enemyQueens = *board->getBitboard(6 * turn + 5);

    u64 allBB = *board->getBitboard(14);
    u64 bishopBlockers = allBB & MoveTables::getMask(kingSq, BISHOP);
    u64 rookBlockers = allBB & MoveTables::getMask(kingSq, ROOK);

    u64 pawnAttacks = turn ? ((kingBB & MoveTables::getEdgeMask(6)) << 7) | ((kingBB & MoveTables::getEdgeMask(2)) << 9) 
    : ((kingBB & MoveTables::getEdgeMask(6)) >> 9) | ((kingBB & MoveTables::getEdgeMask(2)) >> 7);

    updateBitboards(move);
    
    if (MoveTables::getKingMoves(kingSq) & enemyKing
        || pawnAttacks & enemyPawns
        || MoveTables::getMagicMoves(kingSq, BISHOP, bishopBlockers) & (enemyBishops | enemyQueens)
        || MoveTables::getKnightMoves(kingSq) & enemyKnights
        || MoveTables::getMagicMoves(kingSq, ROOK, rookBlockers) & (enemyRooks | enemyQueens)) 
        { return false; }
    return true;
}

void Eval::updateBitboards(Move move, bool unmake) {
    Game* board = getBoard();
    
    bool isCapture = (move.cPiece != -1);
    bool isPromotion = (move.pPiece != -1);
    bool turn = move.piece < 6;

    u64 oldSqBB = 1ULL << move.oldSquare;
    u64 newSqBB = 1ULL << move.newSquare;
    u64* pieceBB = board->getBitboard(move.piece);
    u64* friendlyBB = board->getBitboard(turn ? 12 : 13);
    u64* enemyBB = board->getBitboard(turn ? 13 : 12);
    u64* allBB = board->getBitboard(14);
    u64* cPieceBB = board->getBitboard(move.cPiece);
    u64* pPieceBB = board->getBitboard(move.pPiece);
    u64 epBB = (u64)board->getEPFiles() << (turn ? 40 : 16);

    *allBB ^= oldSqBB;
    *friendlyBB ^= oldSqBB;
    *pieceBB ^= oldSqBB;
    
    *friendlyBB ^= newSqBB;
    if (isPromotion) { *pPieceBB ^= newSqBB; }
    else { *pieceBB ^= newSqBB; };
    if (isCapture) {
        if (newSqBB & epBB) {
            *allBB ^= newSqBB;
            turn ? newSqBB >>= 8 : newSqBB <<= 8;
            *allBB ^= newSqBB;
        };
        *enemyBB ^= newSqBB; 
        *cPieceBB ^= newSqBB; 
    }
    else { *allBB ^= newSqBB; };
    if ((move.piece == 0 || move.piece == 6)
        && (abs(move.oldSquare - move.newSquare) == 2)) {
        u64* rookBB = board->getBitboard(move.piece + 4);
        int intSq = (move.oldSquare + move.newSquare) / 2;
        int rookSq = intSq > move.oldSquare ? move.oldSquare + 3 : move.oldSquare - 4;
        *allBB ^= rookSq;
        *allBB ^= intSq;
        *friendlyBB ^= rookSq;
        *friendlyBB ^= intSq;
        *rookBB ^= rookSq;
        *rookBB ^= intSq;
    }
}

std::vector<Move*> Eval::addCastleMoves(int piece) {
    std::vector<Move*> moves;
    Game* board = getBoard();
    bool turn = board->getTurn();

    int castleRights = board->getCastles();
    u64 longMask = 14ULL << (turn ? 0 : 56);
    u64 shortMask = 96ULL << (turn ? 0 : 56);
    u64 allBB = *board->getBitboard(14);
    
    u64 castleBB = turn ? SWCASTLE | LWCASTLE : SBCASTLE | LBCASTLE;
    while (castleBB > 0) {
        int sq = turn ? 4 : 60;
        int newSq = Utils::ctz(castleBB);
        int intSq = (sq + newSq) / 2;

        Move* interMove = new Move(sq, intSq, piece); 
        bool interLegal = checkMoveIsLegal(*interMove);
        if (interLegal) {
            Move* castleMove = new Move(sq, newSq, piece);
            bool legal = checkMoveIsLegal(*castleMove);
            if (legal) { moves.push_back(castleMove); }
        }
        castleBB ^= 1ULL << newSq;
    }
    return moves;
}

void Eval::undoCastleRights() { 
    Game* board = getBoard();
    int n = mCastleHistory.size() - 2;
    int lastRights = mCastleHistory[n];
    mCastleHistory.pop_back();
    board->setCastlingRights(lastRights);
}

void Eval::undoEPFiles() { 
    Game* board = getBoard();
    int n = mEPHistory.size() - 2; 
    int lastFiles = mEPHistory[n];
    mEPHistory.pop_back();
    board->setEPFiles(lastFiles);
}

void Eval::undoFiftyClock() { 
    Game* board = getBoard();
    int n = mFiftyClockHistory.size() - 2;
    int lastClock = mFiftyClockHistory[n];
    mFiftyClockHistory.pop_back();
    board->setFiftyClock(lastClock);
}

} // namespace Engine