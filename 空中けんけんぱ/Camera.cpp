#include "DxLib.h"
#include <vector>
#include "Camera.h"


Camera::Camera()
{
	// グラフィックの描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	//奥行0.1～1000までをカメラの描画範囲とする
	SetCameraNearFar(0.1f, 2000.0f);

    this->Aimpos = VGet(0, 0, 0);
	this->LookPos = VGet(0, 0, 0);
	a = 0;
	flg = false;
	isDistance = false;
}

Camera::~Camera()
{

}

void Camera::Initilize()
{
	a = 0;
	flg = false;
	isDistance = false;

}

void Camera::Update(Player& player)
{
	VECTOR AimPos = VGet(0, 0, 0);
	VECTOR LookPos = VGet(0,0,0);
	//通常時はplayerをカメラが追いかける
	if (!player.cameraFlg && !player.GetIsAcceleration())
	{
		AimPos = VGet(0, 25, player.GetPos().z - 30.0f);
		LookPos = VGet(0, -10, player.GetPos().z -15);
	}
	//落ちた時はplayerが戻ってくる位置をleapを使って追いかける
	else if (!player.GetIsAcceleration())
	{
		AimPos = VGet(0, 25, player.GetCameraPos().z - 30.0f);
 		LookPos = VGet(0, -10, player.GetCameraPos().z - 15);
	}
	else if (player.GetIsAcceleration())
	{
		AimPos = VGet(0, 15, player.GetPos().z - 30.0f);
		LookPos = VGet(0, -10, player.GetPos().z - 15);
		isDistance = true;
	}

	VECTOR SubAimPos = VSub(AimPos, this->Aimpos);
	VECTOR SubLookPos = VSub(LookPos, this->LookPos);
	VECTOR scaleAimPos = VGet(0, 0, 0);
	VECTOR scaleLookPos = VGet(0, 0, 0);

	//通常時は普通のカメラ移動
	if (!player.cameraFlg && !player.GetIsAcceleration() && !isDistance)
	{
		scaleAimPos = SubAimPos;
		scaleLookPos = SubLookPos;
	}
	//playerが落ちた時、lerpを使用しカメラ移動
	else
	{
		scaleAimPos = VScale(SubAimPos, 0.1f);
		scaleLookPos = VScale(SubLookPos, 0.1f);
		//カメラが元の位置に戻った場合にカメラ移動を通常に戻す
		if (scaleAimPos.z == SubAimPos.z && scaleLookPos.z == SubLookPos.z)
		{
			isDistance = false;
		}
	}
	
	this->Aimpos = VAdd(this->Aimpos, scaleAimPos);
	this->LookPos= VAdd(this->LookPos, scaleLookPos);

    SetCameraPositionAndTarget_UpVecY(this->Aimpos,this->LookPos);

}

void Camera::TitleUpdate()
{
	SetCameraPositionAndTarget_UpVecY(VGet(0, 0, -20),
		VGet(0, 0, -5));
}