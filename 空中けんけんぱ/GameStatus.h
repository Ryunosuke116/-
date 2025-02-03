#pragma once
#include"player.h"
#include "Map.h"
#include"Camera.h"
#include "BackGround.h"
#include "CommonFunction.h"

enum GameStatus
{
    TITLE,
    START,
    GAME,
    CALC,
    RESULT
};

//‹¤’Ê‚ÌƒNƒ‰ƒX
class CommonVariable
{
public:
    bool isPushKey;
    bool isBlackOut;
    int blackHandle;
    int pushSound;
    int alpha;
    int add;
    CommonVariable();

    void FlagSwitch(bool& flag);
};




class GameScreen
{
public:

    GameScreen();
    ~GameScreen();

    void Initilize();
    void Update(int& gameStatus, Player& player, CommonFunction& CommonFunction, Map& map);
    void Draw(Player& player);
    int GetMin() { return min; }
    int GetSec() { return sec; }
    int GetMsec() { return msec; }
    CommonFunction* commonFunction;
private:
    int time;
    int setTime;
    int sec;        //•b
    int min;        //•ª
    int msec;       //ƒ~ƒŠ•b
    int GoolHandle;
    int mapGauge;
    int playerFace;
    int playerFace_Y;
    int x1, x2, y1, y2;
    int timer;
    int startTime;
    int gameBGM;
    int goolSound;

    bool isSetStartTime;
    bool isGameBGM;
};

class ResultScreen
{
public:
    ResultScreen();
    ~ResultScreen();
    void Initilize(int& gameStatus,const int min, const int sec, const int msec);
    void Update(int& gameStatus, Player& player, Camera& camera);
    void Draw(const int min, const int sec, const int msec);
private:
    int sec[12];        //•b
    int min[12];        //•ª
    int msec[12];       //ƒ~ƒŠ•b
    int secSum[12];
    int msecSum[12];
    int BackHandle;
    int score_Y;
    int red, green, blue;
    int fafa;
    int alpha;
    int add;
    int resultBGM;
    bool isColor;
    bool isResultBGM;
};

class StartScreen
{
public:
    StartScreen();
    ~StartScreen();

    void Initilize();
    void Update(int& gameStatus, Player& player, GameScreen& gameScreen);
    void Draw(Player& player);
    CommonFunction* commonFunction;
private:
    int x1, x2, y1, y2;
    int timer;
    int startTime;
    int numberHandle[4];
    int tutorialHandle[3];
    int HandleCount;
    int tutorialCount;
    int countDownSound;
    bool isSetStartTime;
    bool isTutorial;
    bool isSound;

};

class TitleScreen
{
public:
    TitleScreen();
    ~TitleScreen();

    void Initilize();
    void Update(int& gameStatus,Player& player, Map& map, StartScreen& startScreen, Camera& camera, GameScreen& gameScreen, BackGround& backGround);
    void Draw();
private:
    int skyHandle;
    int backGroundX;
    int titleHandle;
    int titleBGM;
    int alpha;
    int add;
    bool isTitleBGM;
};