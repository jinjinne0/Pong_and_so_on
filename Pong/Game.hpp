#pragma once

#include "SDL/SDL.h"

//--------------------------------
//Macro definition
//--------------------------------


//--------------------------------
//Type definition
//--------------------------------

struct Color{
    Color(short int, short int, short int, short int);
    short int R,G,B,A;//0~255のRGBA値
};

struct Vector2{
    Vector2(float, float);

    Vector2 operator + (const Vector2&) const;
    Vector2 operator - (const Vector2&) const;
    Vector2 operator * (float d) const;
    
    float norm()const;
    float abs()const;

    float x;
    float y;
};
typedef Vector2 Point2;

//形、サイズ、色を持つ長方形
class Rectangle{
public:
    Rectangle(float, float, float, float);

    //members
    Point2 coordinate; //coordinate of the center of rectangle
    Vector2 vell;      //vellocity of the rectangle
    float width;
    float height;
    Color color;
};

class Game{
public:
    Game();
    bool Initialize();
    void RunLoop();
    void Shutdown();

private:
    //Helper functions for Runloop
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    //Window created by SDL
    SDL_Window* mWindow;
    //Renderer for 2D drawing
    SDL_Renderer* mRenderer;
    //Should we continue game?
    bool mIsRunning;
    //Should we pause game?
    bool mIsPaused;
    //ticks count
    Uint32 mTicksCount;

    //Paddle
    Rectangle mPaddle;
    //Ball
    Rectangle mBall;

};


//--------------------------------
//Prototype declaration
//--------------------------------

