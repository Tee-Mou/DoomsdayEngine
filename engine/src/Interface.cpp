#include "../inc/Interface.h"
#include <iostream>
#include <ostream>
#include <string>

using namespace std;

void Interface::runInterface() {
    while (true) {
        bool cont = getCommand();
        if (!cont) break;
    }
}

bool Interface::getCommand() {
    string command;
    cout << "# ";
    getline(cin, command);
    cout << "Provided Command: " << command << std::endl;
    if (command == "exit") { return false; }
    return true;
}