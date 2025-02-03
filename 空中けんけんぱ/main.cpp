#include "DxLib.h"
#include<vector>
#include"player.h"
#include"Camera.h"
#include "Map.h"
#include"CommonFunction.h"
#include "GameStatus.h"
#include "BackGround.h"

/// <summary>
/// メイン関数
/// </summary>
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	// 画面モードのセット
	ChangeWindowMode(false);
	SetGraphMode(1600, 900, 16);

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;	// エラーが起きたら直ちに終了
	}

	// Ｚバッファを有効にする
	SetUseZBuffer3D(TRUE);

	// Ｚバッファへの書き込みを有効にする
	SetWriteZBuffer3D(TRUE);


	Player* player = new Player();
	Camera* camera = new Camera();
	Map*	map	   = new Map();
	CommonFunction* commonFunction = new CommonFunction();
	TitleScreen* titleScreen = new TitleScreen();
	StartScreen* startScreen = new StartScreen();
	GameScreen* gameScreen = new GameScreen();
	ResultScreen* resultScreen = new ResultScreen();
	BackGround* backGround = new BackGround();
	int gameStatus = TITLE;
	//初期化
	player->Initilize();
	titleScreen->Initilize();
	backGround->Initilize();

	//ロード
	map->Load();

	// エスケープキーが押されるかウインドウが閉じられるまでループ
	LONGLONG frameTime = 0;
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		frameTime = GetNowHiPerformanceCount();

		//アップデート
		if (gameStatus == TITLE)
		{
			titleScreen->Update(gameStatus,*player,*map,*startScreen,*camera,*gameScreen,*backGround);
			camera->TitleUpdate();
			map->Update(*commonFunction);
		}
		else if (gameStatus == START)
		{
			camera->Update(*player);
			startScreen->Update(gameStatus, *player,*gameScreen);
		}
		else if (gameStatus == GAME)
		{
			gameScreen->Update(gameStatus,*player,*commonFunction,*map);
			player->Update(*map,*commonFunction,gameStatus);
			map->Update(*commonFunction);
			camera->Update(*player);
			backGround->Update(*player);
		}
		else if (gameStatus == CALC)
		{
			resultScreen->Initilize(gameStatus, gameScreen->GetMin(), gameScreen->GetSec(), gameScreen->GetMsec());
		}
		else if (gameStatus == RESULT)
		{
			map->Update(*commonFunction);
			resultScreen->Update(gameStatus,*player,*camera);
		}
		
		//画面の初期化
		ClearDrawScreen();

		//描画
		if (gameStatus == TITLE)
		{
			map->Draw();
			backGround->Draw();
			player->Draw();
			titleScreen->Draw();
		}
		else if (gameStatus == START)
		{
			map->Draw();
			backGround->Draw();
			player->Draw();
			startScreen->Draw(*player);
		}
		else if (gameStatus == GAME)
		{
			map->Draw();
			backGround->Draw();
			player->Draw();
			gameScreen->Draw(*player);
		}
		else if (gameStatus == RESULT)
		{
			map->ResultDraw();
			resultScreen->Draw(gameScreen->GetMin(), gameScreen->GetSec(), gameScreen->GetMsec());
		}

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();

		// 雑に60fps固定
		while ((GetNowHiPerformanceCount() - frameTime) < 16667) {}
	}	

	delete(player);
	delete(map);
	delete(camera);
	delete(commonFunction);
	delete(titleScreen);
	delete(startScreen);
	delete(gameScreen);
	delete(resultScreen);
	delete(backGround);

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}