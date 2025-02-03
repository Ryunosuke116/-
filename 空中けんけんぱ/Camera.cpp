#include "DxLib.h"
#include <vector>
#include "Camera.h"


Camera::Camera()
{
	// �O���t�B�b�N�̕`���𗠉�ʂɃZ�b�g
	SetDrawScreen(DX_SCREEN_BACK);

	//���s0.1�`1000�܂ł��J�����̕`��͈͂Ƃ���
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
	//�ʏ펞��player���J�������ǂ�������
	if (!player.cameraFlg && !player.GetIsAcceleration())
	{
		AimPos = VGet(0, 25, player.GetPos().z - 30.0f);
		LookPos = VGet(0, -10, player.GetPos().z -15);
	}
	//����������player���߂��Ă���ʒu��leap���g���Ēǂ�������
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

	//�ʏ펞�͕��ʂ̃J�����ړ�
	if (!player.cameraFlg && !player.GetIsAcceleration() && !isDistance)
	{
		scaleAimPos = SubAimPos;
		scaleLookPos = SubLookPos;
	}
	//player�����������Alerp���g�p���J�����ړ�
	else
	{
		scaleAimPos = VScale(SubAimPos, 0.1f);
		scaleLookPos = VScale(SubLookPos, 0.1f);
		//�J���������̈ʒu�ɖ߂����ꍇ�ɃJ�����ړ���ʏ�ɖ߂�
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