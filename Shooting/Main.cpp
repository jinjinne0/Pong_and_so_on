
#include "Game.hpp"
#include "SDL/SDL.h"

int main(int argv, char** argc){
    Game game;
    bool init_result = game.Initialize();
    if(init_result){
        game.RunLoop();
    }
    game.Shutdown();

    return 0;
}