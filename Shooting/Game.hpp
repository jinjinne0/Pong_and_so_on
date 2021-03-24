#pragma once

#include "SDL/SDL.h"
#include "stdio.h"
#include <list>

//--------------------------------
//Macro definition
//--------------------------------


//--------------------------------
//Type definition
//--------------------------------

struct Vector2{
    Vector2(float, float);
    float x, y;
};

typedef Vector2 Point2;

struct Color{
    Color(short int, short int, short int, short int);
    short int R, G, B, A;
};

//class for dynamic object
class PongObject{
public:
    //Coordinatee of the centeer of object
    Point2 coordinate;
    float width, height;
    //vellocity
    Vector2 vell;
    Color color;
};

//Class that controlls the game
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

    //SDL window
    SDL_Window* mWindow;
    //SDL Renderer for 2D drawing
    SDL_Renderer* mRenderer;
    //Should we continue the game?
    bool mIsRunning;
    //now Paused?
    bool mIsPaused;
    //Ticks Count
    Uint32 mTicksCount;
    //ticks while paused
    Uint32 mPausingTicks;

    //specific for Shooting
    PongObject mShooter;
    //List of bullets
    std::list<PongObject> mBullets;
    //List of enemy
    std::list<PongObject> mEnemies;


};

//--------------------------------
//Prototype definition
//--------------------------------

