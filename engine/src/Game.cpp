#include <stdexcept>
#include <cmath>
#include <string>
#include <vector>

#include "../inc/Game.h"
#include "../inc/Utils.h"

namespace Engine {
    Game::Game (str fen) {
        setFen(fen);
    };

    void Game::processPieceLocations(str locFen) {
        int square = 56;
        for (int i = 0; i < locFen.size(); ++i) {
            u64* allBb = getBitboard(14);
            u64* colourBb;
            u64* pieceBb;
            char thisChar = locFen[i];
            if (thisChar == '/') { square -= 16; continue; }
            else if (thisChar > 48 && thisChar < 58) { square += (thisChar - 48); continue; }
            switch (thisChar)
            {
                case 'K':
                    pieceBb = getBitboard(0);
                    colourBb = getBitboard(12);
                    break;
                case 'P':
                    pieceBb = getBitboard(1);
                    colourBb = getBitboard(12);
                    break;
                case 'B':
                    pieceBb = getBitboard(2);
                    colourBb = getBitboard(12);
                    break;
                case 'N':
                    pieceBb = getBitboard(3);
                    colourBb = getBitboard(12);
                    break;
                case 'R':
                    pieceBb = getBitboard(4);
                    colourBb = getBitboard(12);
                    break;
                case 'Q':
                    pieceBb = getBitboard(5);
                    colourBb = getBitboard(12);
                    break;
                case 'k':
                    pieceBb = getBitboard(6);
                    colourBb = getBitboard(13);
                    break;
                case 'p':
                    pieceBb = getBitboard(7);
                    colourBb = getBitboard(13);
                    break;
                case 'b':
                    pieceBb = getBitboard(8);
                    colourBb = getBitboard(13);
                    break;
                case 'n':
                    pieceBb = getBitboard(9);
                    colourBb = getBitboard(13);
                    break;
                case 'r':
                    pieceBb = getBitboard(10);
                    colourBb = getBitboard(13);
                    break;
                case 'q':
                    pieceBb = getBitboard(11);
                    colourBb = getBitboard(13);
                    break;
                default:
                    throw std::invalid_argument( "received invalid character in FEN string.");
            }
            *allBb ^= 1ULL << square;
            *colourBb ^= 1ULL << square;
            *pieceBb ^= 1ULL << square;
            square++;
        };
    };

    void Game::processCastlingRights(str castleFen) {
        castlingRights = 0;
        if (castleFen.size() == 4) { castlingRights = 15; return; };
        if (castleFen == "-") {castlingRights = 0; return; }
        int rights = 0;
        for(int i = 0; i < 3; i++ ) {
            char c = castleFen[i];
            if (c == 'K') { rights ^= 0b0010; };
            if (c == 'Q') { rights ^= 0b0001; };
            if (c == 'k') { rights ^= 0b1000; };
            if (c == 'q') { rights ^= 0b0100; };
        };
        castlingRights = rights;
        return;
    }

    void Game::processEnPassantFiles(str epFen) {
        enPassantFiles = 0;
        char file = epFen[0];
        if (file == '-') { enPassantFiles = 0; return; };
        enPassantFiles = 1ULL << (file - 97);
        return;
    }

    void Game::processCurrentTurn(str turnFen, str moveFen) {
        halfMove = 0;
        halfMove += (std::stoi(moveFen) - 1) * 2;
        halfMove += (turnFen == "w" ? 0 : 1);
    }

    void Game::processFiftyMoveClock(str fiftyMoveFen) {
        fiftyClock = 0;
        int length = fiftyMoveFen.size();
        for (int i = 0; i < length; ++i) {
            fiftyClock += ((int)fiftyMoveFen[length - (1 + i)] - 48) * pow(10, i);
        }
    }

    void Game::setFen(str fen) {
        mFen = fen;
        for (int i = 0; i < 15; i++) {
            pieceLocations[i] = 0;
        };

        int endLocFen = fen.find(' ');
        int endTurnFen = fen.find(' ', endLocFen + 1);
        int endCastleFen = fen.find(' ', endTurnFen + 1);
        int endEpFen = fen.find(' ', endCastleFen + 1);
        int endFiftyMoveFen = fen.find(' ', endEpFen + 1);

        str locFen = fen.substr(0, endLocFen);
        str turnFen = fen.substr(endLocFen + 1, endTurnFen - (endLocFen + 1));
        str castleFen = fen.substr(endTurnFen + 1, endCastleFen - (endTurnFen + 1));
        str epFen = fen.substr(endCastleFen + 1, endEpFen - (endCastleFen + 1));
        str fiftyMoveFen = fen.substr(endEpFen + 1, endFiftyMoveFen - (endEpFen + 1));
        str moveFen = fen.substr(endFiftyMoveFen + 1, std::string::npos);
        processPieceLocations(locFen);
        processCastlingRights(castleFen);
        processEnPassantFiles(epFen);
        processCurrentTurn(turnFen, moveFen);
        processFiftyMoveClock(fiftyMoveFen);
    }

    str Game::calculateFEN() {
        std::vector<char> pieceChar {
            'K', 'P', 'N', 'B', 'R', 'Q',
            'k', 'p', 'n', 'b', 'r', 'q'
        };
        str pieceStr = "";
        u64* allBB = getBitboard(14);
        int row = 7;
        int col = 0;
        int emptySquares = 0;
        while (row >= 0) {
            bool foundPiece = false;
            int sq = row * 8 + col;
            u64 sqBB = 1ULL << sq;
            if (sqBB & *allBB) {
                for (int i = 0; i < 12; i++) {
                    u64* pieceBB = getBitboard(i);
                    if (sqBB & *pieceBB) {
                        foundPiece = true;
                        if (emptySquares != 0) {
                            pieceStr += std::to_string(emptySquares);
                            emptySquares = 0;
                        }
                        pieceStr += pieceChar[i]; 
                        break;
                    }
                }
            }
            if (!foundPiece) emptySquares++;
            col++;
            if (col == 8) {
                if (emptySquares != 0) {
                    pieceStr += std::to_string(emptySquares);
                    emptySquares = 0;
                }
                if (row > 0) pieceStr += '/';
                row -= 1; 
                col = 0;
            }
        }

        str turnStr = getTurn() ? "w" : "b";
        str castlingStr = "";
        int castleRights = getCastles();
        if (castleRights == 0) castlingStr = "-";
        else {
            if (castleRights & 2) castlingStr += 'K';
            if (castleRights & 1) castlingStr += 'Q';
            if (castleRights & 8) castlingStr += 'k';
            if (castleRights & 4) castlingStr += 'q';
        }
        str enPassantStr = "";
        int epFile = getEPFiles();
        if (epFile == 0) enPassantStr = "-";
        else {
            enPassantStr +=  (char)(Utils::ctz(epFile) + 97);
            enPassantStr += (char)((getHalfMove() % 2 == 0) ? '6' : '3');
        }
        str fiftyMoveStr = std::to_string(getFiftyMoveClock());
        str fullMoveStr = std::to_string((int)floor(getHalfMove() / 2) + 1);

        str fenStr = (
            pieceStr + " " + 
            turnStr + " " + 
            castlingStr + " " +
            enPassantStr + " " +
            fiftyMoveStr + " " +
            fullMoveStr
        );
        
        return fenStr;
    }

} //namespace Engine