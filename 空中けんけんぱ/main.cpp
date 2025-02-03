#include "DxLib.h"
#include<vector>
#include"player.h"
#include"Camera.h"
#include "Map.h"
#include"CommonFunction.h"
#include "GameStatus.h"
#include "BackGround.h"

/// <summary>
/// ���C���֐�
/// </summary>
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	// ��ʃ��[�h�̃Z�b�g
	ChangeWindowMode(false);
	SetGraphMode(1600, 900, 16);

	// �c�w���C�u��������������
	if (DxLib_Init() == -1)
	{
		return -1;	// �G���[���N�����璼���ɏI��
	}

	// �y�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(TRUE);

	// �y�o�b�t�@�ւ̏������݂�L���ɂ���
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
	//������
	player->Initilize();
	titleScreen->Initilize();
	backGround->Initilize();

	//���[�h
	map->Load();

	// �G�X�P�[�v�L�[��������邩�E�C���h�E��������܂Ń��[�v
	LONGLONG frameTime = 0;
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		frameTime = GetNowHiPerformanceCount();

		//�A�b�v�f�[�g
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
		
		//��ʂ̏�����
		ClearDrawScreen();

		//�`��
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

		// ����ʂ̓��e��\��ʂɔ��f������
		ScreenFlip();

		// �G��60fps�Œ�
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

	// �c�w���C�u�����̌�n��
	DxLib_End();

	// �\�t�g�̏I��
	return 0;
}