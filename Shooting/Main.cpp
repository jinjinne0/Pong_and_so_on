
#include "Game.hpp"
#include "SDL/SDL.h"
#include "time.h"

int main(int argv, char** argc){
    srand(time(NULL));
    Game game;
    bool init_result = game.Initialize();
    if(init_result){
        game.RunLoop();
    }
    game.Shutdown();

    return 0;
}