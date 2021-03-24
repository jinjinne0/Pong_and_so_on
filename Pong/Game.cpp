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
const int kWindowWidth = 1024;
const int kWindowHeight = 768;
const int kThickness = 15;
const float kPaddleHeight = 100.0f; 
const float kAcceleration_ps = 20.0f;

//--------------------------------
//Function definition
//--------------------------------

Color::Color(short int R = 0, short int G = 0, short int B = 0, short int A = 0)
:R(R)
,G(G)
,B(B)
,A(A)
{}

Vector2::Vector2(float x = 0.0f, float y = 0.0f)
:x(x)
,y(y)
{}

Vector2 Vector2::operator+(const Vector2 &other) const {
    return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(const Vector2 &other) const {
    return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator*(float d) const {
    return Vector2(x * d, y * d);
}

float Vector2::norm()const{
    return x*x + y*y;
}
float Vector2::abs()const{
    return sqrt(norm());
}

Rectangle::Rectangle(float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f)
:width(width)
,height(height)
{
    coordinate.x = x;
    coordinate.y = y;
    color = Color(0,0,0,0); //デフォルトでは黒
    vell.x = 0.0f;    //デフォルトの速度はゼロ
    vell.y = 0.0f;
}

Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mIsRunning(true)
,mIsPaused(true)
,mTicksCount(0)
{

}


bool Game::Initialize(){

    //Initialize SDL
    int is_sdl_init = SDL_Init(SDL_INIT_VIDEO);
    if(is_sdl_init != 0){
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    //Create Window
    mWindow = SDL_CreateWindow(
        "Pong",  //Window title
        100,     //Top left x-coordinate of window
        100,     //Top left y-coordinate of window
        kWindowWidth,    //Width of window
        kWindowHeight,     //Height of window
        0        //Flags (0 for normal)
    );
    if(!mWindow){
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    //Create renderer
    mRenderer =  SDL_CreateRenderer(
        mWindow,  //Window to create for renderer for
        -1,       //-1 for normal
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if(!mRenderer){
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    //Initialize paddle
    mPaddle = Rectangle(
        kThickness,           //x-coordinate of the center of paddle
        kWindowHeight/2,  //y-coordinate of the center of paddle
        kThickness,          //width of paddle
        static_cast<int>(kPaddleHeight)        //height of paddle
    );
    mPaddle.color = Color(255,0,0,255);
    
    mBall = Rectangle(
        kWindowWidth/2,
        kWindowHeight/2,
        kThickness,
        kThickness
    );
    mBall.color = Color(255,255,255,255);
    mBall.vell = Vector2(-200.0f, 235.0f);

    return true;
}

void Game::RunLoop(){
    while(mIsRunning){
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printf("       Your score is %dpt\n", static_cast<int>(SDL_GetTicks()/1000.0f));
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
}

void Game::ProcessInput(){

    //if there are any quit type event, end game 
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
            mIsRunning = false;
        }
    }

    ////Process Input from the keyboard
    const Uint8* state = SDL_GetKeyboardState(NULL);    

    //If escape key is pushed, end game
    if(state[SDL_SCANCODE_ESCAPE]){
        mIsRunning = false;
    }
    //If 's' key is pushed, pause game
    if(state[SDL_SCANCODE_S]){
        mIsPaused = true;
    }
    //If 'a' key is pushed, start or restart game
    if(state[SDL_SCANCODE_A]){
        mIsPaused = false;
    }

    //controll paddle
    mPaddle.vell.y = 0;
    if(state[SDL_SCANCODE_E]){
        mPaddle.vell.y--;
    }
    if(state[SDL_SCANCODE_D]){
        mPaddle.vell.y++;
    }

}

void Game::UpdateGame(){

    //limiting frame rate for deltatime
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount+16));

    //set delta time to second
    float deltatime = (SDL_GetTicks() - mTicksCount)/1000.0f;
    //upper limit of deltatime
    if(deltatime > 0.05f){
        deltatime = 0.05f;
    }

    //update ticks count
    mTicksCount = SDL_GetTicks();

    //If pause, we won`t update game
    if(mIsPaused){
        return;
    }

    //update paddle
    mPaddle.vell.y *= (300.0f + (kAcceleration_ps*mTicksCount)/1000.0f);
    mPaddle.coordinate.y = mPaddle.coordinate.y + (mPaddle.vell.y * deltatime);
    //Make sure the paddle doesn`t go off the screen
    if(mPaddle.coordinate.y < (kThickness + kPaddleHeight/2.0f)){
        mPaddle.coordinate.y = kThickness + kPaddleHeight/2.0f;
    }
    if(mPaddle.coordinate.y > (kWindowHeight - (kPaddleHeight/2.0f + kThickness))){
        mPaddle.coordinate.y = kWindowHeight - (kPaddleHeight/2.0f + kThickness);
    }

    //update ball
    mBall.vell = mBall.vell * (1 + (kAcceleration_ps*deltatime/mBall.vell.abs()));
    mBall.coordinate = mBall.coordinate + (mBall.vell * deltatime);

    //Did the ball collide with walls?
    //with top wall
    if(mBall.coordinate.y <= (kThickness + mBall.height/2.0f) && mBall.vell.y < 0.0f){
        mBall.vell.y *= -1.0f;
    }
    //with bottom wall
    if(mBall.coordinate.y >= (kWindowHeight - (kThickness + mBall.height/2.0f)) && mBall.vell.y > 0.0f){
        mBall.vell.y *= -1.0f;
    }
    //with right wall
    if(mBall.coordinate.x >= (kWindowWidth - (kThickness + mBall.width/2.0f)) && mBall.vell.x > 0.0f){
        mBall.vell.x *= -1.0f;
    }

    //Did the ball interact with paddle?
    float y_diff = mBall.coordinate.y - mPaddle.coordinate.y;
    y_diff = (y_diff >= 0.0f) ? y_diff : -y_diff;
    if(
        y_diff <= (kPaddleHeight / 2.0f) &&
        (mBall.coordinate.x - mPaddle.coordinate.x) <= (mPaddle.width + mBall.width)/2.0f &&
        (mBall.coordinate.x - mPaddle.coordinate.x) >= ((mPaddle.width + mBall.width)/2.0f - 5.0f) &&
        mBall.vell.x < 0.0f
    ){
        mBall.vell.x *= -1.0f;
    }

    //Did the ball go off the screen?
    if(mBall.coordinate.x < 0.0f){
        mIsRunning = false;
    }
}

void Game::GenerateOutput(){

    //Draw background with green
    SDL_SetRenderDrawColor(
        mRenderer,  //target renderer
        0,          //R
        255,        //G
        0,          //B
        255         //A
    );
    SDL_RenderClear(mRenderer);

    //Draw walls with black
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    //top wall
    SDL_Rect wall{
        0,             //top left x-coordinate
        0,             //top left y-coordinate
        kWindowWidth,  //width
        kThickness     //height
    };
    SDL_RenderFillRect(mRenderer, &wall);
    //bottom wall
    wall.y = kWindowHeight - kThickness;
    SDL_RenderFillRect(mRenderer, &wall);
    //right wall
    wall.x = kWindowWidth - kThickness;
    wall.y = 0;
    wall.w = kThickness;
    wall.h = kWindowHeight;
    SDL_RenderFillRect(mRenderer, &wall);

    //draw paddle
    SDL_Rect paddle{static_cast<int>(mPaddle.coordinate.x - mPaddle.width/2.0f), static_cast<int>(mPaddle.coordinate.y - mPaddle.height/2.0f)
                    , static_cast<int>(mPaddle.width), static_cast<int>(mPaddle.height)
    };
    SDL_SetRenderDrawColor(mRenderer, mPaddle.color.R, mPaddle.color.G, mPaddle.color.B, mPaddle.color.A);
    SDL_RenderFillRect(mRenderer, &paddle);

    //drraw ball
    SDL_Rect ball{static_cast<int>(mBall.coordinate.x - mBall.width/2.0f), static_cast<int>(mBall.coordinate.y - mBall.height/2.0f)
                , static_cast<int>(mBall.width), static_cast<int>(mBall.height)
    };
    SDL_SetRenderDrawColor(mRenderer, mBall.color.R, mBall.color.G, mBall.color.B, mBall.color.A);
    SDL_RenderFillRect(mRenderer, &ball);

    SDL_RenderPresent(mRenderer);
}

void Game::Shutdown(){
    SDL_DestroyWindow(mWindow);
    SDL_DestroyRenderer(mRenderer);
    SDL_Quit();
}