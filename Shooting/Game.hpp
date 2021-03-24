#pragma once

#include "SDL/SDL.h"

//--------------------------------
//Macro definition
//--------------------------------


//--------------------------------
//Type definition
//--------------------------------

class Game{
public:
    Game::Game();
    bool Initialize();
    void RunLoop();
    void Shutdown();

private:
    //Helper functions for RunLoop
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    //Should we continue the game?
    bool mIsRunning;

};

//--------------------------------
//Prototype definition
//--------------------------------

