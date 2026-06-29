#pragma once

struct Move
{
    Move(
        int oldSq, int newSq, int p, int cP = -1, int pP = -1, float scr = 0
    ) : oldSquare(oldSq), newSquare(newSq), piece(p), cPiece(cP), pPiece(pP), score(scr) {};
    int oldSquare;
    int newSquare;
    int piece;
    int cPiece;
    int pPiece;
    bool checks = false;
    float score;
};
