#include "inc/Interface.h"
#include "inc/Eval.h"
using namespace Engine;

int main() {
    Game* game = new Game();
    Eval* eval = new Eval(game);
    Interface* interface = new Interface(eval);

    interface->runInterface();

    return 0;
}