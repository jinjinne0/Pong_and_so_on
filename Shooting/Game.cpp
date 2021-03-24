#include "Game.hpp"
#include "SDL/SDL.h"

//--------------------------------
//Macro definition(not revealed)
//--------------------------------


//--------------------------------
//Type definition(not revealed)
//--------------------------------


//--------------------------------
//global variable definition
//--------------------------------


//--------------------------------
//Function definition
//--------------------------------

Game::Game()
{}

bool Game::Initialize(){}

void Game::RunLoop(){
    while(mIsRunning){
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::Shutdown(){}

void Game::ProcessInput(){}

void Game::UpdateGame(){}

void Game::GenerateOutput(){}