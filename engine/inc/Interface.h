#include "Eval.h"
#include <string>
#include <tuple>
#include <map>


using str = std::string;
using tuple = std::tuple<std::string, std::string>;

class Interface
{
    using func = bool (Interface::*)(str);
    public:
        Interface(Engine::Eval* eval) : eval(eval) {};
        void runInterface();
        tuple getCommand();
        bool executeCommand(tuple input);
        bool executeFEN(str arg);
        bool executePerft(str arg);
        bool executeAnalyse(str arg);
        bool executeReset(str arg);
        bool executeHelp(str arg);
        bool executeExit(str arg);
        std::map<str, func> commandMap = {
            {"fen", &Interface::executeFEN},
            {"perft", &Interface::executePerft},
            {"analyse", &Interface::executeAnalyse},
            {"reset", &Interface::executeReset},
            {"help", &Interface::executeHelp},
            {"exit", &Interface::executeExit},
        };

    private:
        Engine::Eval* eval;
};