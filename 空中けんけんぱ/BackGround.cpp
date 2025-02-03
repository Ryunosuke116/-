#include <math.h>
#include <vector>
#include "DxLib.h"
#include"player.h"
#include "BackGround.h"

BackGround::BackGround()
{
	startTileHandle = MV1LoadModel("material/hex_grass.mv1");
	endTileHandle = MV1LoadModel("material/hex_grass.mv1");
	homeHandle = MV1LoadModel("material/building_home_A_blue.mv1");
	airShipHandle = MV1LoadModel("material/Low-Poly_airship.mv1");
	castleHandle = MV1LoadModel("material/building_castle_blue.mv1");
	planeHandle = MV1LoadModel("material/plane.mv1");
}

BackGround::~BackGround()
{
	// モデルのアンロード.
	MV1DeleteModel(startTileHandle);
	MV1DeleteModel(endTileHandle);
	MV1DeleteModel(homeHandle);
	MV1DeleteModel(airShipHandle);
	MV1DeleteModel(castleHandle);
	MV1DeleteModel(planeHandle);
}

void BackGround::Initilize()
{
	airShipPos_Z = -6 * 10;
	planePos_Z = -6 * 10;

	startTilePos = VGet(0, -9.9f, 12.5f);
	endTilePos = VGet(0, -9.9f, -972.5f);
	castlePos = VGet(0, -10, -972.2f);
	homePos = VGet(0, -10, 14);
	airShipPos = VGet(airShipPos_X, -30, airShipPos_Z);
	planePos = VGet(planePos_X, -30, planePos_Z);

	///////////////////////////////
	//向きを調整
	///////////////////////////////
	rotate = VGet(0, 10.99f, 0);
	//右向き
	planeRotata = VGet(0, 3.8f, 0);
	//左向き
	airShipRotate = VGet(0, 3.54f, 0);

	//大きさ調整
	MV1SetScale(startTileHandle, VGet(0.15f, 0.01f, 0.15f));
	MV1SetScale(endTileHandle, VGet(0.15f, 0.01f, 0.15f));
	MV1SetScale(homeHandle, VGet(0.3f, 0.3f, 0.3f));
	MV1SetScale(castleHandle, VGet(0.035f, 0.025f, 0.035f));
	MV1SetScale(airShipHandle, VGet(0.03f, 0.03f, 0.03f));
	MV1SetScale(planeHandle, VGet(0.03f, 0.03f, 0.03f));


	// 3Dのポジション設定
	MV1SetPosition(startTileHandle, startTilePos);
	MV1SetPosition(endTileHandle, endTilePos);

	// 3Dのポジション設定
	MV1SetPosition(homeHandle, homePos);

	// 3Dのポジション設定
	MV1SetPosition(castleHandle, castlePos);

	// 3Dのポジション設定
	MV1SetPosition(airShipHandle, airShipPos);
	MV1SetPosition(planeHandle, planePos);

	//3Dの向き設定
	MV1SetRotationXYZ(startTileHandle, rotate);
	MV1SetRotationXYZ(endTileHandle, rotate);
	MV1SetRotationXYZ(airShipHandle, airShipRotate);
	MV1SetRotationXYZ(planeHandle, planeRotata);

	airShipAppearanceCount = 20;
	planeAppearanceCount = 10;
	isAirShipDirection = false;
	isPlaneDirection = false;
}

void BackGround::Update(const Player& player)
{
	//planeの出現タイミング
	if (player.GetCol() >= planeAppearanceCount)
	{
		//位置設定
		planePos_Z = -6 * (player.GetCol() + 30);
		//次の出現場所の設定
		planeAppearanceCount += 40;

		if (!isPlaneDirection) 
		{
			//左向き
			planeRotata = VGet(0, 3.8f, 0);
			//初期位置設定
			planePos_X = -80;

			isPlaneDirection = true;
		}
		else
		{
			//右向き
			planeRotata = VGet(0, -3.8f, 0);
			//初期位置設定
			planePos_X = 80;

			isPlaneDirection = false;
		}

		MV1SetRotationXYZ(planeHandle, planeRotata);
	}

	//trueの時右進行
	if (isPlaneDirection)
	{
		planePos_X += 0.3f;
		planePos_Z += 0.3f;
	}
	//falseの時左進行
	else
	{
		planePos_X -= 0.3f;
		planePos_Z += 0.3f;
	}

	planePos = VGet(planePos_X, -30, planePos_Z);
	// 3Dのポジション設定
	MV1SetPosition(planeHandle, planePos);

	//airShipの出現タイミング
	if (player.GetCol() >= airShipAppearanceCount)
	{
		//位置設定
		airShipPos_Z = -6 * (player.GetCol() + 15);
		//次の出現場所の設定
		airShipAppearanceCount += 40;

		if (!isAirShipDirection)
		{
			//右向き
			airShipRotate = VGet(0, 0.4f, 0);
			//初期位置設定
			airShipPos_X = -GetRand(50);
		
			isAirShipDirection = true;
		}
		else
		{
			//左向き
			airShipRotate = VGet(0, 3.54f, 0);
			//初期位置設定
			airShipPos_X = GetRand(50);

			isAirShipDirection = false;
		}
		MV1SetRotationXYZ(airShipHandle, airShipRotate);
	}
	//trueの時右進行
	if (isAirShipDirection)
	{
		airShipPos_X += 0.1f;
	}
	//falseの時左進行
	else
	{
		airShipPos_X -= 0.1f;
	}
	
	airShipPos = VGet(airShipPos_X, -30, airShipPos_Z);
	// 3Dのポジション設定
	MV1SetPosition(airShipHandle, airShipPos);
}

void BackGround::Draw()
{
	// ３Ｄモデルの描画
	MV1DrawModel(startTileHandle);

	// ３Ｄモデルの描画
	MV1DrawModel(homeHandle);

	// ライティングの計算をしないように設定を変更
	SetUseLighting(FALSE);
	MV1DrawModel(airShipHandle);
	SetUseLighting(TRUE);

	MV1DrawModel(planeHandle);

	// ３Ｄモデルの描画
	MV1DrawModel(castleHandle);

	// ３Ｄモデルの描画
	MV1DrawModel(endTileHandle);
}