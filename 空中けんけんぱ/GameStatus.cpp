#include "DxLib.h"
#include <stdio.h>
#include<vector>
#include"player.h"
#include"Camera.h"
#include "Map.h"
#include"CommonFunction.h"
#include "BackGround.h"
#include "GameStatus.h"

CommonVariable::CommonVariable()
{
    blackHandle = LoadGraph("material/black.png");
    isPushKey = false;
    isBlackOut = false;
    alpha = 0;
    add = 4;
}

void CommonVariable::FlagSwitch(bool& flag)
{

}

CommonVariable* commonVariable = new CommonVariable;

/// <summary>
/// ゲームプレイ画面
/// </summary>
GameScreen::GameScreen()
{
    GoolHandle = LoadGraph("material/GOAL.png");
    mapGauge = LoadGraph("material/mapGauge.png");
    playerFace = LoadGraph("material/playerFace.png");
    //音楽セット
    gameBGM = LoadSoundMem("sound/y015.mp3");
    goolSound = LoadSoundMem("sound/gool.mp3");
}

GameScreen::~GameScreen()
{

}

void GameScreen::Initilize()
{
    setTime = GetNowCount();

    x1 = 800;
    x2 = 800;
    y1 = 450;
    y2 = 450;
    timer = 3000;
    playerFace_Y = 100;         //メモ：620
    isSetStartTime = false;
    isGameBGM= false;
}

void GameScreen::Update(int& gameStatus,Player& player, CommonFunction& commonFunction,Map& map)
{
    if (!isGameBGM)
    {
        PlaySoundMem(gameBGM, DX_PLAYTYPE_LOOP);
        isGameBGM = true;
    }
    if (!player.GetIsGool())
    {
        time = GetNowCount() - setTime;

        //秒,分、ミリ秒を計算
        sec = time / 1000;
        min = sec / 60;
        sec -= min * 60;

        // ミリ秒算出(１０ミリ秒単位っぽい)
        msec = (time - sec * 1000 - min * 60000) / 10;

        int movingFace = 650 / map.GetMapLayer();
        playerFace_Y = movingFace * commonFunction.GetCol();

    }
    if (player.GetIsGool())
    {
        if (!isSetStartTime)
        {
            PlaySoundMem(goolSound, DX_PLAYTYPE_BACK);
            StopSoundMem(gameBGM);
            commonFunction.TimerSet(startTime);
            isSetStartTime = true;
        }

        if (!(GetNowCount() - startTime < timer))
        {
            //画面を暗転させる
            commonVariable->alpha += commonVariable->add;

            //暗転したらリザルト画面へ
            if (commonVariable->alpha >= 256)
            {
                commonVariable->add = -8;
                commonVariable->isBlackOut = true;
                gameStatus = CALC;
            }
        }

        if (x1 > 320)
        {
            x1 -= 16;
            y1 -= 7;
            x2 += 16;
            y2 += 7;
        }

    }
}
void GameScreen::Draw(Player& player)
{
    SetFontSize(50);
    DrawFormatString(10, 30, GetColor(0, 0, 0), "[ %02d'%02d\"%02d ]", min, sec, msec);
    if (player.GetIsGool())
    {
        DrawExtendGraph(x1, y1, x2, y2, GoolHandle, true);
    }

    DrawGraph(1300, 100, mapGauge, TRUE);
    DrawGraph(1308, 100 + playerFace_Y, playerFace, TRUE);

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, commonVariable->alpha);
    DrawGraph(0, 0, commonVariable->blackHandle, TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// <summary>
/// タイトル
/// </summary>

TitleScreen::TitleScreen()
{
    titleHandle = LoadGraph("material/title.png");
    commonVariable->blackHandle = LoadGraph("material/black.png");
    titleBGM = LoadSoundMem("sound/y013.mp3");
    commonVariable->pushSound = LoadSoundMem("sound/決定13.mp3");
}

TitleScreen::~TitleScreen()
{

}

void TitleScreen::Initilize()
{
    isTitleBGM = false;
    alpha = 256;
    add = 2;
}

void TitleScreen::Update(int& gameStatus,Player& player,Map& map, StartScreen& startScreen,Camera& camera,GameScreen& gameScreen,BackGround& backGround)
{
    if (!isTitleBGM)
    {
        PlaySoundMem(titleBGM, DX_PLAYTYPE_LOOP);
        isTitleBGM = true;
    }

    //スペースキー入力でゲームスタート
    if (CheckHitKey(KEY_INPUT_SPACE) && commonVariable->alpha <= 0)
    {
        commonVariable->isPushKey = true;
        PlaySoundMem(commonVariable->pushSound, DX_PLAYTYPE_BACK);
    }
   
    if (CheckHitKey(KEY_INPUT_S))
    {
        gameStatus = CALC;
    }

    if (!commonVariable->isBlackOut && commonVariable->isPushKey)
    {
        //画面を暗転させる
        commonVariable->alpha += commonVariable->add;

        //暗転したらゲームスタート
        if (commonVariable->alpha >= 256)
        {
            commonVariable->add = -8;
            commonVariable->isBlackOut = true;
            commonVariable->isPushKey = false;
        }

        if (!commonVariable->isPushKey)
        {
            StopSoundMem(titleBGM);
            gameStatus = START;
            startScreen.Initilize();
            player.Initilize();
            camera.Initilize();
            backGround.Initilize();
            gameScreen.Initilize();
            int count = map.GetMapLayer();
            for (int i = 0; i < count; i++)
            {
                map.MapChipReset(map.GetChip(i), i);
            }
            isTitleBGM = false;
        }
    }

    //画面の暗転を戻す
    if (commonVariable->isBlackOut)
    {
        commonVariable->alpha += commonVariable->add;

        if (commonVariable->alpha <= 0)
        {
            commonVariable->add = 8;
            commonVariable->isBlackOut = false;
        }
    }

    alpha += add;

    if (alpha >= 256)
    {
        add = -2;
    }
    else if(alpha <= 0)
    {
        add = 2;
    }

}

void TitleScreen::Draw()
{
    DrawGraph(220, -70, titleHandle, TRUE);
    SetFontSize(50);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
    DrawString(530,480, "PRESS (SPACE) TO START ", GetColor(0, 0, 0));
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, commonVariable->alpha);
    DrawGraph(0, 0, commonVariable->blackHandle, TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}



/// <summary>
/// リザルト画面
/// </summary>
/// <param name="gameStatus"></param>
/// <param name="min"></param>
/// <param name="sec"></param>
/// <param name="msec"></param>

ResultScreen::ResultScreen()
{
    BackHandle = LoadGraph("material/clearscore.png");
    resultBGM = LoadSoundMem("sound/y014.mp3");
    for (int i = 0; i < 12; i++)
    {
        min[i] = 0;
        sec[i] = 0;
        msec[i] = 0;
        secSum[i] = 0;
    }


}

ResultScreen::~ResultScreen()
{

}

void ResultScreen::Initilize(int& gameStatus,const int min, const int sec, const int msec)
{
    int secSum = 0;
    int msecSum = 0;
    int minTmp, secTmp, msecTmp,secSumTmp,msecSumTmp;
    isColor = false;
    commonVariable->isPushKey = false;
    isResultBGM = false;
    secSum += min * 60;
    secSum += sec;
    secSum += (msec + sec * 1000 + min * 60000) * 10;
    alpha = 256;
    add = 2;


    //最下位よりスコアが高ければ、更新する
    if (this->secSum[11] == 0)
    {
        this->min[11] = min;
        this->sec[11] = sec;
        this->msec[11] = msec;

        this->secSum[11] = secSum;
    }
    else if (this->secSum[11] > secSum)
    {
        this->min[11] = min;
        this->sec[11] = sec;
        this->msec[11] = msec;

        this->secSum[11] = secSum;
    }

    for (int i = 0; i < 12; i++)
    {
       
        for (int j = i + 1; j < 12; j++)
        {
            //
            if (this->secSum[i] == 0)
            {
                minTmp = this->min[i];
                secTmp = this->sec[i];
                msecTmp = this->msec[i];
                secSumTmp = this->secSum[i];

                this->min[i] = this->min[j];
                this->sec[i] = this->sec[j];
                this->msec[i] = this->msec[j];
                this->secSum[i] = this->secSum[j];

                this->min[j] = minTmp;
                this->sec[j] = secTmp;
                this->msec[j] = msecTmp;
                this->secSum[j] = secSumTmp;

            }
            //i番目がj番目よりも小さい時数値を入れ替える
            else  if (this->secSum[i] > this->secSum[j])
            {
                if (this->secSum[j] != 0)
                {
                    minTmp = this->min[i];
                    secTmp = this->sec[i];
                    msecTmp = this->msec[i];
                    secSumTmp = this->secSum[i];

                    this->min[i] = this->min[j];
                    this->sec[i] = this->sec[j];
                    this->msec[i] = this->msec[j];
                    this->secSum[i] = this->secSum[j];

                    this->min[j] = minTmp;
                    this->sec[j] = secTmp;
                    this->msec[j] = msecTmp;
                    this->secSum[j] = secSumTmp;
                }
            }
            
        }
    }

    gameStatus = RESULT;
}

void ResultScreen::Update(int& gameStatus, Player& player,Camera& camera)
{
    if (CheckHitKey(KEY_INPUT_SPACE))
    {
        if (!commonVariable->isPushKey)
        {
            PlaySoundMem(commonVariable->pushSound, DX_PLAYTYPE_BACK);
            StopSoundMem(resultBGM);
        }
        commonVariable->isPushKey = true;
    }
    
    score_Y = 400;

    //フォントの透明度調整
    alpha += add;

    if (alpha >= 256)
    {
        add = -2;
    }
    else if (alpha <= 0)
    {
        add = 2;
    }

    //画面の暗転を戻す
    if (commonVariable->isBlackOut)
    {
        commonVariable->alpha += commonVariable->add;

        if (commonVariable->alpha <= 0)
        {
            commonVariable->add = 8;
            commonVariable->isBlackOut = false;
        }
    }

    if (commonVariable->isPushKey)
    {
        //画面を暗転させる
        commonVariable->alpha += commonVariable->add;

        //暗転したらタイトル画面へ
        if (commonVariable->alpha >= 256)
        {
            commonVariable->add = -8;
            commonVariable->isBlackOut = true;
            gameStatus = TITLE;
            player.Initilize();
            camera.TitleUpdate();
            commonVariable->isPushKey = false;
        }
    }

    if (!isResultBGM)
    {
        PlaySoundMem(resultBGM, DX_PLAYTYPE_LOOP);
        isResultBGM = true;
    }
}

void ResultScreen::Draw(const int min, const int sec, const int msec)
{
    isColor = false;
    DrawGraph(100, 20, BackHandle, TRUE);

    DrawString(250,130, "今回のスコア ", GetColor(0, 255, 0));

    SetFontSize(120);
    DrawFormatString(430, 200, GetColor(255, 255, 255), "[ %02d'%02d\"%02d ]",
        min,sec,msec);

    SetFontSize(50); 
    DrawString(250, 330, "ランキング ", GetColor(0, 255, 0));

    for (int i = 0; i < 6; i++)
    {
        if (this->min[i] == min && this->sec[i] == sec &&
            this->msec[i] == msec && !isColor)
        {
            DrawFormatString(400, score_Y, GetColor(255, 20, 0), "%d", i + 1);
            DrawFormatString(470, score_Y, GetColor(255, 20, 0), "[ %02d'%02d\"%02d ]",
                this->min[i], this->sec[i], this->msec[i]);
            isColor = true;
        }
        else
        {
            DrawFormatString(400, score_Y, GetColor(210, 210, 210), "%d", i + 1);
            DrawFormatString(470, score_Y, GetColor(210, 210, 210), "[ %02d'%02d\"%02d ]",
                this->min[i], this->sec[i], this->msec[i]);
        }
        score_Y += 50;
    }

    score_Y = 400;

    for (int i = 6; i < 12; i++)
    {
        if (this->min[i] == min && this->sec[i] == sec &&
            this->msec[i] == msec && !isColor)
        {
            DrawFormatString(830, score_Y, GetColor(255, 20, 0), "%d", i + 1);
            DrawFormatString(900, score_Y, GetColor(255, 20, 0), "[ %02d'%02d\"%02d ]",
                this->min[i], this->sec[i], this->msec[i]);
            isColor = true;
        }
        else
        {
            DrawFormatString(830, score_Y, GetColor(210, 210, 210), "%d", i + 1);
            DrawFormatString(900, score_Y, GetColor(210, 210, 210), "[ %02d'%02d\"%02d ]",
                this->min[i], this->sec[i], this->msec[i]);
        }
        
        score_Y += 50;
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
    DrawString(520, 720, "PRESS (SPACE) TO TITLE ", GetColor(0, 0, 0));
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, commonVariable->alpha);
    DrawGraph(0, 0, commonVariable->blackHandle, TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// <summary>
/// スタート
/// </summary>

StartScreen::StartScreen()
{
    numberHandle[0] = LoadGraph("material/start.png");
    numberHandle[1] = LoadGraph("material/one.png");
    numberHandle[2] = LoadGraph("material/two.png");
    numberHandle[3] = LoadGraph("material/three.png");
    tutorialHandle[0] = LoadGraph("material/tutorial.png");
    tutorialHandle[1] = LoadGraph("material/tutorial_1.png");
    tutorialHandle[2] = LoadGraph("material/tutorial_2.png");
    countDownSound = LoadSoundMem("sound/Countdown01-2.mp3");

}

StartScreen::~StartScreen()
{
    
}

void StartScreen::Initilize()
{
    x1 = 800;
    x2 = 800;
    y1 = 450;
    y2 = 450;
    timer = 1000;
    HandleCount = 3;
    tutorialCount = 0;
    isSetStartTime = false;
    isTutorial = true;
    isSound = false;
}

void StartScreen::Update(int& gameStatus, Player& player,GameScreen& gameScreen)
{
    if (!isTutorial)
    {
        if (!isSound)
        {
            PlaySoundMem(countDownSound, DX_PLAYTYPE_BACK);
            isSound = true;
        }

        if (!isSetStartTime)
        {
            commonFunction->TimerSet(startTime);
            isSetStartTime = true;
        }

        //カウントごとに文字を徐々に大きくする
        if (!(GetNowCount() - startTime < timer))
        {
            isSetStartTime = false;
            x1 = 800;
            x2 = 800;
            y1 = 450;
            y2 = 450;
            if (HandleCount == 0)
            {
                gameStatus = GAME;
                gameScreen.Initilize();
            }
            HandleCount--;
        }
        else if ((GetNowCount() - startTime < timer))
        {
            if (x1 > 600 && HandleCount >= 1)
            {
                x1 -= 10;
                y1 -= 12;
                x2 += 10;
                y2 += 12;
            }
            else if (x1 > 540 && HandleCount == 0)
            {
                x1 -= 13;
                y1 -= 7;
                x2 += 13;
                y2 += 7;
            }
        }
    }
    else if (isTutorial)
    {
        if (GetMouseInput() & (MOUSE_INPUT_LEFT))
        {
            if (!commonVariable->isPushKey)
            {
                tutorialCount--;
                if (tutorialCount <= 0)
                {
                    tutorialCount = 0;
                }
                commonVariable->isPushKey = true;
            }
        }
        else  if (GetMouseInput() & (MOUSE_INPUT_RIGHT))
        {
            if (!commonVariable->isPushKey)
            {
                tutorialCount++;
                if (tutorialCount >= 2)
                {
                    tutorialCount = 2;
                }
                commonVariable->isPushKey = true;
            }
        }
        else if (CheckHitKey(KEY_INPUT_SPACE) && !commonVariable->isBlackOut)
        {
            if (!commonVariable->isPushKey)
            {
                isTutorial = false;
            }
            commonVariable->isPushKey = true;
        }
        else
        {
            commonVariable->isPushKey = false;
        }

        //画面の暗転を戻す
        if (commonVariable->isBlackOut)
        {
            commonVariable->alpha += commonVariable->add;

            if (commonVariable->alpha <= 0)
            {
                commonVariable->add = 8;
                commonVariable->isBlackOut = false;
            }
        }
    }
}

void StartScreen::Draw(Player& player)
{
    DrawExtendGraph(x1, y1, x2, y2, numberHandle[HandleCount], true);
    if (isTutorial)
    {
        DrawGraph(120, 20, tutorialHandle[tutorialCount], TRUE);
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, commonVariable->alpha);
    DrawGraph(0, 0, commonVariable->blackHandle, TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}