#include <stdexcept>
#include <string>
#include <iostream>
#include "../inc/MoveTables.h"
#include "../inc/Utils.h"
#define TRY_MAGICS 100000

namespace Engine {

    u64 MoveTables::edgeMasks[8];
    u64 MoveTables::bishopMasks[64];
    u64 MoveTables::rookMasks[64];
    u64 MoveTables::kingAttacksTable[64];
    u64 MoveTables::pawnAttacksTableWhite[64];
    u64 MoveTables::pawnAttacksTableBlack[64];
    u64 MoveTables::pawnMovesTableWhite[64];
    u64 MoveTables::pawnMovesTableBlack[64];
    u64 MoveTables::bishopAttacksTable[64][512];
    u64 MoveTables::knightAttacksTable[64];
    u64 MoveTables::rookAttacksTable[64][4096];
    // u64 MoveTables::bishopMagics[64];
    // u64 MoveTables::rookMagics[64];

    void MoveTables::generateEdgeMasks() {
        MoveTables::edgeMasks[0] = 72057594037927935ULL;
        MoveTables::edgeMasks[1] = edgeMasks[0] >> 8;

        u64 edgeMasksTwo = 0ULL;
        u64 edgeMasksThree = 0ULL;
        for (int i = 0; i < 57; i+=8) {
            edgeMasksTwo |= 127ULL << i;
            edgeMasksThree |= 63ULL << i;
        }
        MoveTables::edgeMasks[2] = edgeMasksTwo;
        MoveTables::edgeMasks[3] = edgeMasksThree;

        MoveTables::edgeMasks[4] = ~(255ULL);
        MoveTables::edgeMasks[5] = edgeMasks[4] << 8;
        
        u64 edgeMasksSix = 0ULL;
        u64 edgeMasksSeven = 0ULL;
        for (int i = 0; i < 57; i+=8) {
            edgeMasksSix |= 254ULL << i;
            edgeMasksSeven |= 252ULL << i;
        }
        MoveTables::edgeMasks[6] = edgeMasksSix;
        MoveTables::edgeMasks[7] = edgeMasksSeven;
    }

    void MoveTables::generateBishopMasks() {
        for (int sq = 0; sq < 64; sq++) {
            u64 mask = 0ULL;
            int r = sq / 8;
            int f = sq % 8;

            for (int i = f + 1, j = r + 1; i < 7 && j < 7; i++, j++) { mask |= 1ULL << (i + j * 8); }
            for (int i = f + 1, j = r - 1; i < 7 && j > 0; i++, j--) { mask |= 1ULL << (i + j * 8); }
            for (int i = f - 1, j = r - 1; i > 0 && j > 0; i--, j--) { mask |= 1ULL << (i + j * 8); }
            for (int i = f - 1, j = r + 1; i > 0 && j < 7; i--, j++) { mask |= 1ULL << (i + j * 8); }

            MoveTables::bishopMasks[sq] = mask;
        }
    }

    void MoveTables::generateRookMasks() {
        for (int sq = 0; sq < 64; sq++) {
            u64 mask = 0ULL;
            int r = sq / 8;
            int f = sq % 8;

            for (int i = f + 1; i < 7; i++) { mask |= 1ULL << (i + r * 8); }
            for (int i = f - 1; i > 0; i--) { mask |= 1ULL << (i + r * 8); }
            for (int j = r + 1; j < 7; j++) { mask |= 1ULL << (f + j * 8); }
            for (int j = r - 1; j > 0; j--) { mask |= 1ULL << (f + j * 8); }
        
            MoveTables::rookMasks[sq] = mask;
        }
    }

    void MoveTables::generateMagic(int sq, Piece piece) {
        u64 blockers[4096];
        u64 attacks[4096];
        u64 used[4096];
        u64 mask = getMask(sq, piece);
        int rBits = Utils::csb(mask);
        int blockerPermutations = 1 << rBits;
        for (int i = 0; i < blockerPermutations; i++) {
            blockers[i] = generateBlockers(i, rBits, mask);
            attacks[i] = findSquareAttacks(sq, piece, blockers[i]);
        }
        for (int i = 0; i < TRY_MAGICS; i++) {
            u64 tryMagic = Utils::genMagic();
            if (Utils::csb((mask * tryMagic) & 0xFF00000000000000ULL) < 7) { continue; };
            for (int i = 0; i < 4096; i++) { used[i] = 0ULL; }
            int fail = 0;
            for (int b = 0, fail = 0; !fail && b < blockerPermutations; b++) {
                unsigned int hash = (unsigned int)((tryMagic * blockers[b]) >> (64 - rBits));
                if (used[hash] == 0ULL) {
                    used[hash] = attacks[b];
                }
                if (used[hash] != attacks[b]) { 
                    fail = 1;
                    break;
                }
            }
            if (!fail) {
                // piece == ROOK ? rookMagics[sq] = tryMagic : bishopMagics[sq] = tryMagic;
                break;
            }
        }
    }

    int MoveTables::magicHash(int sq, Piece piece, u64 blockers) {
        int rBits = getRelevantBits(sq, piece);
        u64 magicNumber = getMagic(sq, piece);
        int hash = (magicNumber * blockers) >> (64 - rBits);
        return hash;
    }

    void MoveTables::generateKingAttacksTable() {
        for (int sq = 0; sq < 64; sq++) {
            u64 sqBb = 1ULL << sq;
            u64 moveBb = 0ULL;
            moveBb ^= (edgeMasks[0] & sqBb) << 8;
            moveBb ^= (edgeMasks[0] & edgeMasks[2] & sqBb) << 9;
            moveBb ^= (edgeMasks[2] & sqBb) << 1;
            moveBb ^= (edgeMasks[2] & edgeMasks[4] & sqBb) >> 7;
            moveBb ^= (edgeMasks[4] & sqBb) >> 8;
            moveBb ^= (edgeMasks[4] & edgeMasks[6] & sqBb) >> 9;
            moveBb ^= (edgeMasks[6] & sqBb) >> 1;
            moveBb ^= (edgeMasks[6] & edgeMasks[0] & sqBb) << 7;
            kingAttacksTable[sq] = moveBb;
        };
    }

    void MoveTables::generatePawnTables() {
        for (int sq = 0; sq < 64; sq++) {
            u64 pawnAttacksWhite = 0ULL;
            u64 pawnAttacksBlack = 0ULL;
            u64 pawnMovesWhite = 0ULL;
            u64 pawnMovesBlack = 0ULL;
            u64 sqBB = 1ULL << sq;
            pawnAttacksWhite |= (sqBB & edgeMasks[6]) << 7;
            pawnAttacksWhite |= (sqBB & edgeMasks[2]) << 9;
            pawnAttacksBlack |= (sqBB & edgeMasks[6]) >> 9;
            pawnAttacksBlack |= (sqBB & edgeMasks[2]) >> 7;
            pawnMovesWhite |= sqBB << 8;
            pawnMovesWhite |= (sqBB & 0xFF00ULL) << 16;
            pawnMovesBlack |= sqBB >> 8;
            pawnMovesBlack |= (sqBB & 0xFF000000000000ULL) >> 16;
            MoveTables::pawnAttacksTableWhite[sq] = pawnAttacksWhite;
            MoveTables::pawnAttacksTableBlack[sq] = pawnAttacksBlack;
            MoveTables::pawnMovesTableWhite[sq] = pawnMovesWhite;
            MoveTables::pawnMovesTableBlack[sq] = pawnMovesBlack;
        }
    }

    void MoveTables::generateKnightAttacksTable() {
        for (int sq = 0; sq < 64; sq++) {
            u64 sqBb = 1ULL << sq;
            u64 moveBb = 0ULL;
            moveBb ^= (edgeMasks[1] & edgeMasks[2] & sqBb) << 17;
            moveBb ^= (edgeMasks[3] & edgeMasks[0] & sqBb) << 10;
            moveBb ^= (edgeMasks[3] & edgeMasks[4] & sqBb) >> 6;
            moveBb ^= (edgeMasks[5] & edgeMasks[2] & sqBb) >> 15;
            moveBb ^= (edgeMasks[5] & edgeMasks[6] & sqBb) >> 17;
            moveBb ^= (edgeMasks[7] & edgeMasks[4] & sqBb) >> 10;
            moveBb ^= (edgeMasks[7] & edgeMasks[0] & sqBb) << 6;
            moveBb ^= (edgeMasks[1] & edgeMasks[6] & sqBb) << 15;
            knightAttacksTable[sq] = moveBb;
        };
    }

    u64 MoveTables::generateBlockers(int index, int relevantBits, u64 mask) {
        u64 blockers = 0ULL;
        for (int count = 0; count < relevantBits; count++)
        {
            int square = Utils::ctz(mask);
            if (mask & (1ULL << square)) {
                mask &= ~(1ULL << square);
            };
            if (index & (1 << count))
                blockers |= (1ULL << square);
        }
        return blockers;
    }

    void MoveTables::generateMagicAttacksTable(Piece piece) {
        for (int sq = 0; sq < 64; sq++) {
            u64 mask = getMask(sq, piece);
            int rBits = getRelevantBits(sq, piece);
            int blockerPermutations = 1 << rBits;
            for (int i = 0; i < blockerPermutations; i++) {
                u64 blockers = generateBlockers(i, rBits, mask);
                int hash = magicHash(sq, piece, blockers);
                u64 attacks = findSquareAttacks(sq, piece, blockers);
                if (piece == ROOK) { rookAttacksTable[sq][hash] = attacks; }
                else { bishopAttacksTable[sq][hash] = attacks; }
            }
        };
    };

    u64 MoveTables::findSquareAttacks(int sq, Piece piece, u64 blockers) {
        u64 moves = 0ULL;
        u64 rayMoves = 0ULL;
        u64 mask = getMask(sq, piece);
        int f, r, i, j, fail;
        f = sq % 8;
        r = sq / 8;
        
        if (piece == ROOK) {
            for (i = f + 1, rayMoves = 0ULL; i <= 7; i++) { rayMoves |= 1ULL << (i + r * 8); if (rayMoves & blockers) { break; } }
            moves |= rayMoves;
            for (i = f - 1, rayMoves = 0ULL; i >= 0; i--) { rayMoves |= 1ULL << (i + r * 8); if (rayMoves & blockers) { break; } }
            moves |= rayMoves;
            for (j = r + 1, rayMoves = 0ULL; j <= 7; j++) { rayMoves |= 1ULL << (f + j * 8); if (rayMoves & blockers) { break; } }
            moves |= rayMoves;
            for (j = r - 1, rayMoves = 0ULL; j >= 0; j--) { rayMoves |= 1ULL << (f + j * 8); if (rayMoves & blockers) { break; } }
            moves |= rayMoves;
        }
        else {
            for (i = f + 1, j = r + 1, rayMoves = 0ULL; i <= 7 && j <= 7; i++, j++) { rayMoves |= 1ULL << (i + j * 8); if (rayMoves & blockers) { break; } }
            moves |= rayMoves;
            for (i = f + 1, j = r - 1, rayMoves = 0ULL; i <= 7 && j >= 0; i++, j--) { rayMoves |= 1ULL << (i + j * 8); if (rayMoves & blockers) { break; } }
            moves |= rayMoves;
            for (i = f - 1, j = r - 1, rayMoves = 0ULL; i >= 0 && j >= 0; i--, j--) { rayMoves |= 1ULL << (i + j * 8); if (rayMoves & blockers) { break; } }
            moves |= rayMoves;
            for (i = f - 1, j = r + 1, rayMoves = 0ULL; i >= 0 && j <= 7; i--, j++) { rayMoves |= 1ULL << (i + j * 8); if (rayMoves & blockers) { break; } }
            moves |= rayMoves;
        }
        return moves;
    }

    u64 MoveTables::getKingMoves(int sq) {
        if (sq >= 0 && sq <= 63) { return kingAttacksTable[sq]; };
        throw std::invalid_argument("sq must be in range 0 < sq < 63, got" + std::to_string(sq));
    };

    u64 MoveTables::getPawnAttacks(int sq, Colour colour) {
        if (sq >= 0 && sq <= 63) { return colour ? pawnAttacksTableWhite[sq] : pawnAttacksTableBlack[sq]; };
        throw std::invalid_argument("sq must be in range 0 < sq < 63, got" + std::to_string(sq));
    };

    u64 MoveTables::getPawnMoves(int sq, Colour colour) {
        if (sq >= 0 && sq <= 63) { return colour ? pawnMovesTableWhite[sq] : pawnMovesTableBlack[sq]; };
        throw std::invalid_argument("sq must be in range 0 < sq < 63, got" + std::to_string(sq));
    };

    u64 MoveTables::getKnightMoves(int sq) {
        if (sq >= 0 && sq <= 63) { return knightAttacksTable[sq]; };
        throw std::invalid_argument("sq must be in range 0 < sq < 63, got" + std::to_string(sq));
    };

    u64 MoveTables::getMagicMoves(int sq, Piece piece, u64 blockers) {
        if (sq >= 0 && sq <= 63) {
            int hash = magicHash(sq, piece, blockers);
            return piece == ROOK ? rookAttacksTable[sq][hash] : bishopAttacksTable[sq][hash];
        }
        throw std::invalid_argument("sq must be in range 0 < sq < 63, got" + std::to_string(sq));
    }

} // namespace Engine