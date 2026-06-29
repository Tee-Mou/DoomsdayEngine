#include "../inc/Interface.h"
#include <iostream>
#include <string>

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
    if (
        command == "perft" ||
        command == "fen" ||
        command == "analyse"
    )  arg = input.substr(endCommand + 1, str::npos);
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
    return (this->*commandMap[command])(arg);
}

bool Interface::executeFEN(str arg) {
    eval->setFen(arg);
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
    std::cout << "\tperft <depth> - Run perft test to a specified depth." << std::endl;
    std::cout << "\tanalyse <depth> - Run positiona analysis to a specified depth." << std::endl;
    std::cout << "\treset - Set the board to its initial state." << std::endl;
    std::cout << "\thelp - Show this message." << std::endl;
    return true;
}

bool Interface::executeExit(str arg) {
    std::exit(0);
    return true;
}