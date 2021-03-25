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
const float kBulletSpeed = 300.0f;
const int kWindowWidth = 1024;
const int kWindowHeight = 768;
const int kThickness = 15;
const float kEps = 1e-9; // 
const Color kBulletColor = Color(255, 255, 255, 255);//White
const Color kBackColor = Color(0, 0, 0, 123);        //half-transparent Black
const Color kEnemyColor = Color(255, 0, 0, 255);     //Red
const Color kShooterColor = Color(255, 217, 0, 255); //Yellow
const Color kWallColor = Color(175, 175, 176, 255);  //Silver gray
const float kEnemyInterval_s = 1.0f;//the interval between time enemies come(unit is second)

const PongObject kTopWall(
    Point2(kWindowWidth/2.0f, kThickness/2.0f),  //center-coordinate of top wall
    kWindowWidth,                                //width of top wall
    static_cast<float>(kThickness),              //height of top wall
    Vector2(0.0f, 0.0f),                         //vellocity of top wall
    kWallColor                                   //Color of top wall
);
const PongObject kBottomWall(
    Point2(kWindowWidth/2.0f, kWindowHeight - kThickness/2.0f),
    kWindowWidth,                                
    static_cast<float>(kThickness),              
    Vector2(0.0f, 0.0f),                         
    kWallColor   
);

//--------------------------------
//Function definition
//--------------------------------

Vector2::Vector2(float x = 0.0f, float y = 0.0f):x(x),y(y){}

Vector2 Vector2::operator+(const Vector2& other) const {
    return Vector2(x + other.x, y + other.y);
}
Vector2 Vector2::operator*(float d) const{
    return Vector2(x*d, y*d);
}

Color::Color(short int R = 0, short int G = 0, short int B = 0, short int A =0)
:R(R), G(G), B(B), A(A)
{}

PongObject::PongObject(){}
PongObject::PongObject(Point2 coordinate, float width, float height, Vector2 vell, Color color)
:coordinate(coordinate)
,width(width)
,height(height)
,vell(vell)
,color(color)
{}

SDL_Rect PongObect_To_SDLRect(const PongObject& target){
    return SDL_Rect{
        static_cast<int>(target.coordinate.x - target.width/2.0f),
        static_cast<int>(target.coordinate.y - target.height/2.0f),
        static_cast<int>(target.width),
        static_cast<int>(target.height)
    };
}

void DrawPongObject(SDL_Renderer* renderer_ptr, const PongObject& object){
    SDL_Rect object_to_draw;
    SDL_SetRenderDrawColor(renderer_ptr, object.color.R, object.color.G, object.color.B, object.color.A);
    object_to_draw = PongObect_To_SDLRect(object);
    SDL_RenderFillRect(renderer_ptr, &object_to_draw);
}

//TODO: 簡単な変数の初期化
Game::Game()
:mIsRunning(true)
,mIsPaused(true)
,mTicksCount(0)
,mPausingTicks(0)
,mWindow(nullptr)
,mRenderer(nullptr)
,mLastShootTime_ms(0)
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

    //Initialize shooter
    mShooter.coordinate = Point2(kThickness, kWindowHeight/2.0f);
    mShooter.width = static_cast<float>(kThickness);
    mShooter.height = static_cast<float>(kThickness);
    mShooter.vell = Vector2(0.0f, 0.0f);
    mShooter.color = kShooterColor;

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
        if(event.type == SDL_QUIT){
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

    //K-key to shoot a bullet
    //there is idling time for next bullet
    if(state[SDL_SCANCODE_K] && (mLastShootTime_ms + 2*kThickness*1000.0f/kBulletSpeed) < SDL_GetTicks()){
        PongObject new_bullet(
            mShooter.coordinate + Point2(kThickness/2.0f, 0.0f),
            kThickness,
            kThickness,
            Vector2(300.0f, 0.0f),
            kBulletColor
        );
        mBullets.push_back(new_bullet);
        //update last time of shooting
        mLastShootTime_ms = SDL_GetTicks();
    }

}

void Game::UpdateGame(){
    //get delta time and set it to seconds
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));
    float deltatime = (SDL_GetTicks() - mTicksCount)/1000.0f;

    if(deltatime > 0.05f){
        deltatime = 0.05f;
    }

    //Update mTicksCount
    mTicksCount = SDL_GetTicks();
    //If game is paused, stop updategame.
    if(mIsPaused){
        //update ticks count while pausing
        mPausingTicks += deltatime * 1000.0f;
        return;
    }

    //update shooter`s coordinate
    mShooter.coordinate.y = mShooter.coordinate.y + mShooter.vell.y*deltatime;

    //new enemy
    if(SDL_GetTicks() > mLastEnemyTime_ms + kEnemyInterval_s*1000.0f){
        PongObject new_enemy(
            Point2(static_cast<float>(kWindowWidth), static_cast<float>(random()%1000)),
            kThickness,
            kThickness,
            Vector2(-200.0f - (rand()%100), (rand()%600) - 300),//set new enemy`s max vellocity 
            kEnemyColor
        );
        mEnemies.push_back(new_enemy);
        //update the time new enemy appeared last time
        mLastEnemyTime_ms = SDL_GetTicks();
    }
    //update enemies` coordinates
    for(auto &x: mEnemies){
        x.coordinate = x.coordinate + x.vell*deltatime;
    }

    //update Bullet coordinate
    for(auto &x: mBullets){
        x.coordinate.x = x.coordinate.x + x.vell.x*deltatime;
    }

    //Did any enemy collide with somethig?
    for(auto &x: mEnemies){
        //If an enemy go off the screen, end game
        if(x.coordinate.x < 0.0f){
            mIsRunning = false;
        }

        //TODO: If an enemy collide with the shooter, end game
        if(){}

        //TODO: any enemy that collide with a bullet, disapper.Then, also, the bullet disapper
        if(){}

        //With top wall?
        if(
            (x.coordinate.y - x.height/2.0f) <= (kTopWall.coordinate.y + kTopWall.height/2.0f) &&
            x.vell.y < 0.0f
        ){
            x.vell.y *= -1.0f;
        }
        //With bottom wall?
        else if(
            (x.coordinate.y + x.height/2.0f) >= (kBottomWall.coordinate.y - kBottomWall.height/2.0f) &&
            x.vell.y > 0.0f
        ){
            x.vell.y *= -1.0f;
        }
    } 


}

void Game::GenerateOutput(){

    //draw background
    SDL_SetRenderDrawColor(mRenderer, kBackColor.R, kBackColor.G, kBackColor.B, kBackColor.A);
    SDL_RenderClear(mRenderer);

    //draw top wall
    DrawPongObject(mRenderer, kTopWall);
    //draw bottom wall
    DrawPongObject(mRenderer, kBottomWall);
    //draw shooter
    DrawPongObject(mRenderer, mShooter);
    //Enemies
    for(auto x : mEnemies){
        DrawPongObject(mRenderer, x);
    }

    //Bullets
    for(auto x : mBullets){
        DrawPongObject(mRenderer, x);
    }
    //switch back buffer
    SDL_RenderPresent(mRenderer);
}