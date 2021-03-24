#include "Game.hpp"

//--------------------------------
//Macro definition(not revealed)
//--------------------------------


//--------------------------------
//Type definition(not revealed)
//--------------------------------


//--------------------------------
//global variable definition
//--------------------------------
const float kShooterSpeed = 400.0f; //pixels per seconds
const int kWindowWidth = 1024;
const int kWindowHeight = 768;
const float kEps = 1e-9; // 

//--------------------------------
//Function definition
//--------------------------------

Vector2::Vector2(float x = 0.0f, float y = 0.0f):x(x),y(y){}

Color::Color(short int R = 0, short int G = 0, short int B = 0, short int A =0)
:R(R), G(G), B(B), A(A)
{}

//TODO: 簡単な変数の初期化
Game::Game()
:mIsRunning(true)
,mIsPaused(true)
,mTicksCount(0)
,mPausingTicks(0)
,mWindow(nullptr)
,mRenderer(nullptr)
{}

//If initialization success, return true. If not, return false
bool Game::Initialize(){

    //Initialize SDL
    int sdl_init = SDL_Init(SDL_INIT_VIDEO);
    if(sdl_init != 0){
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    //Create SDL Window
    mWindow = SDL_CreateWindow(
        "Shooting",  //title for window
        100,        //top left x-coordinate
        100,        //top left y-coordinate
        kWindowWidth,       //width
        kWindowHeight,        //height
        0  //0 for default
    );
    if(!mWindow){
        SDL_Log("Failed to create SDL window: %s", SDL_GetError());
        return false;
    }

    //Create SDL Renderer for 2D drawing
    mRenderer = SDL_CreateRenderer(
        mWindow,  //SDL Window to draw
        -1,       //-1 for normal
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if(!mRenderer){
        SDL_Log("Failed to create SDL Renderer: %s", SDL_GetError());
        return false;
    }

    //TODO:Initialize shooter
    mShooter.coordinate = Vector2();
    mShooter.width;
    mShooter.vell;
    mShooter.color = Color();

    //TODO: Prepare for game loop

    //success to initialize the game
    return true;
}

void Game::RunLoop(){

    while(mIsRunning){
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }

    //display your point
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
    printf("       Your point is %dpt\n", (mTicksCount - mPausingTicks)/1000);
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
}

void Game::Shutdown(){
    SDL_DestroyWindow(mWindow);
    SDL_DestroyRenderer(mRenderer);
    SDL_Quit();

    //TODO:Initialize()で初期化などしたものを排除
    //動的に確保したメモリも削除
}

void Game::ProcessInput(){

    //If there QUIT_event, end game
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type = SDL_QUIT){
            mIsRunning = false;
        }
    }

    //get the state of keyboard
    const Uint8* state = SDL_GetKeyboardState(NULL);

    // Escape key get the game end
    if(state[SDL_SCANCODE_ESCAPE]){
        mIsRunning = false;
    }

    //Setting S-key to pause、A-key to restart the game
    if(state[SDL_SCANCODE_A]){
        mIsPaused = false;
    }
    if(state[SDL_SCANCODE_S]){
        mIsPaused = true;
    }

    //E-key : slide shooter up 
    //D-key : slide shooter down
    short int shooter_direction = 0;
    if(state[SDL_SCANCODE_E]){
        shooter_direction--;
    }
    if(state[SDL_SCANCODE_D]){
        shooter_direction++;
    }
    mShooter.vell.y = shooter_direction * kShooterSpeed;

    //TODO: Kでbullet発射

}

void Game::UpdateGame(){
    //TODO: deltatimeの取得
    //TODO: mTicksCountの更新
    //TODO: Pauseしてるなら終了.PausingTicksの更新
    //TODO: Shooterの更新
    //TODO: Enemyの更新(mIsPausingの更新)
    //TODO: Bulletの更新(Enemyの削除も)
}

void Game::GenerateOutput(){
    //TODO: 壁
    //TODO: Shooter
    //TODO: Enemy
    //TODO: Bullets
    //バッファの取り替え
}