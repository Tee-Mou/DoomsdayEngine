#include "Eval.h"
#include <list>
#include <string>
#include <tuple>
#include <map>
#include <vector>


using str = std::string;
using tuple = std::tuple<std::string, std::string>;

namespace Engine {
    
    class Interface
    {
        using func = bool (Interface::*)(str);
        public:
            Interface(Engine::Eval* eval) : eval(eval) {};
            void runInterface();
            tuple getCommand();
            bool executeCommand(tuple input);
            bool executeFEN(str arg);
            bool executePrintFEN(str arg);
            bool executeMove(str arg);
            bool executePerft(str arg);
            bool executeAnalyse(str arg);
            bool executeReset(str arg);
            bool executeHelp(str arg);
            bool executeExit(str arg);
            std::map<str, func> commandMap = {
                {"fen", &Interface::executeFEN},
                {"state", &Interface::executePrintFEN},
                {"move", &Interface::executeMove},
                {"perft", &Interface::executePerft},
                {"analyse", &Interface::executeAnalyse},
                {"reset", &Interface::executeReset},
                {"help", &Interface::executeHelp},
                {"exit", &Interface::executeExit},
            };
            std::vector<str> argCommands = {
                "fen",
                "move",
                "perft",
                "analyse"
            };
    
        private:
            Engine::Eval* eval;
    };
    
} // namespace Engine
