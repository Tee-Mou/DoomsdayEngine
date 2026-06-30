#include "../inc/Interface.h"
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

namespace Engine {

    void Interface::runInterface() {
        while (true) {
            tuple result = getCommand();
            executeCommand(result);
        }
    }
    
    tuple Interface::getCommand() {
        str input;
        str command = "";
        str arg = "";
        std::cout << "# ";
        std::getline(std::cin, input);
    
        int endCommand = input.find(' ');
        command = input.substr(0, endCommand);
        if (std::find(argCommands.begin(), argCommands.end(), command) != argCommands.end()) {
            arg = input.substr(endCommand + 1, str::npos);
        }
        tuple result = {command, arg};
        return result;
    }
    
    bool Interface::executeCommand(tuple input) {
        str command = std::get<0>(input);
        str arg = std::get<1>(input);
        bool commandValid = commandMap.count(command);
        if (!commandValid){
            std::cout << "Unknown command: " << command << std::endl;
            return executeHelp("");
        }
        bool success = (this->*commandMap[command])(arg);
        if (success) {
            std::cout << "Successfully executed " << command << " " << arg << std::endl;
            return true;
        }
        else {
            std::cout << "Execution failed: " << command << " " << arg << std::endl;
            return false;
        }
    }
    
    bool Interface::executeFEN(str arg) {
        eval->setFen(arg);
        return true;
    }
    
    bool Interface::executePrintFEN(str arg) {
        Game* game = eval->getBoard();
        std::cout << "Current FEN: " << eval->getBoard()->getFen() << std::endl;
        return true;
    }
    
    bool Interface::executeMove(str arg) {
        int sqFrom = (int(arg[0]) - 97) + (int(arg[1]) - 49) * 8;
        int sqTo = (int(arg[2]) - 97) + (int(arg[3]) - 49) * 8;
        Move* relevantMove;
    
        std::vector<Move*> moves = eval->findLegalMoves();
        std::vector<Move*>::iterator it;
        bool found = false;
        for (it = moves.begin(); it != moves.end(); ++it) {
            Move* move = *it;
            int oldSq = move->oldSquare;
            int newSq = move->newSquare;
            if (oldSq == sqFrom && newSq == sqTo) {
                relevantMove = move;
                found = true;
                break;
            }
        }
        if (!found) {
            std::cout << "Could not find legal move: " << arg << std::endl;
            return false;
        }
        eval->makeMove(*relevantMove);

        str newFen = eval->getBoard()->calculateFEN();
        eval->setFen(newFen);

        return true;
    }
    
    bool Interface::executePerft(str arg) {
        int depth = std::stoi(arg);
        std::cout << eval->perft(depth) << std::endl; 
        return true;
    }
    
    bool Interface::executeAnalyse(str arg) {
        int depth = std::stoi(arg);
        eval->evalAlphaBeta(depth);
        return true;
    }
    
    bool Interface::executeReset(str arg) {
        eval->setFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        return true;
    }
    
    bool Interface::executeHelp(str arg) {
        std::cout << "Commands:" << std::endl;
        std::cout << "\tfen <FEN> - Set the evaluator's FEN string." << std::endl;
        std::cout << "\tstate - Display the current FEN string." << std::endl;
        std::cout << "\tmove <oldSquare><newSquare> - Make a move from one square to another." << std::endl;
        std::cout << "\tperft <depth> - Run perft test to a specified depth." << std::endl;
        std::cout << "\tanalyse <depth> - Run positiona analysis to a specified depth." << std::endl;
        std::cout << "\treset - Set the board to its initial state." << std::endl;
        std::cout << "\thelp - Show this message." << std::endl;
        std::cout << "\texit - Quit the engine." << std::endl;
        return true;
    }
    
    bool Interface::executeExit(str arg) {
        std::exit(0);
        return true;
    }

} // namespace Engine
