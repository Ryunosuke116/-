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
	// ���f���̃A�����[�h.
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
	//�����𒲐�
	///////////////////////////////
	rotate = VGet(0, 10.99f, 0);
	//�E����
	planeRotata = VGet(0, 3.8f, 0);
	//������
	airShipRotate = VGet(0, 3.54f, 0);

	//�傫������
	MV1SetScale(startTileHandle, VGet(0.15f, 0.01f, 0.15f));
	MV1SetScale(endTileHandle, VGet(0.15f, 0.01f, 0.15f));
	MV1SetScale(homeHandle, VGet(0.3f, 0.3f, 0.3f));
	MV1SetScale(castleHandle, VGet(0.035f, 0.025f, 0.035f));
	MV1SetScale(airShipHandle, VGet(0.03f, 0.03f, 0.03f));
	MV1SetScale(planeHandle, VGet(0.03f, 0.03f, 0.03f));


	// 3D�̃|�W�V�����ݒ�
	MV1SetPosition(startTileHandle, startTilePos);
	MV1SetPosition(endTileHandle, endTilePos);

	// 3D�̃|�W�V�����ݒ�
	MV1SetPosition(homeHandle, homePos);

	// 3D�̃|�W�V�����ݒ�
	MV1SetPosition(castleHandle, castlePos);

	// 3D�̃|�W�V�����ݒ�
	MV1SetPosition(airShipHandle, airShipPos);
	MV1SetPosition(planeHandle, planePos);

	//3D�̌����ݒ�
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
	//plane�̏o���^�C�~���O
	if (player.GetCol() >= planeAppearanceCount)
	{
		//�ʒu�ݒ�
		planePos_Z = -6 * (player.GetCol() + 30);
		//���̏o���ꏊ�̐ݒ�
		planeAppearanceCount += 40;

		if (!isPlaneDirection) 
		{
			//������
			planeRotata = VGet(0, 3.8f, 0);
			//�����ʒu�ݒ�
			planePos_X = -80;

			isPlaneDirection = true;
		}
		else
		{
			//�E����
			planeRotata = VGet(0, -3.8f, 0);
			//�����ʒu�ݒ�
			planePos_X = 80;

			isPlaneDirection = false;
		}

		MV1SetRotationXYZ(planeHandle, planeRotata);
	}

	//true�̎��E�i�s
	if (isPlaneDirection)
	{
		planePos_X += 0.3f;
		planePos_Z += 0.3f;
	}
	//false�̎����i�s
	else
	{
		planePos_X -= 0.3f;
		planePos_Z += 0.3f;
	}

	planePos = VGet(planePos_X, -30, planePos_Z);
	// 3D�̃|�W�V�����ݒ�
	MV1SetPosition(planeHandle, planePos);

	//airShip�̏o���^�C�~���O
	if (player.GetCol() >= airShipAppearanceCount)
	{
		//�ʒu�ݒ�
		airShipPos_Z = -6 * (player.GetCol() + 15);
		//���̏o���ꏊ�̐ݒ�
		airShipAppearanceCount += 40;

		if (!isAirShipDirection)
		{
			//�E����
			airShipRotate = VGet(0, 0.4f, 0);
			//�����ʒu�ݒ�
			airShipPos_X = -GetRand(50);
		
			isAirShipDirection = true;
		}
		else
		{
			//������
			airShipRotate = VGet(0, 3.54f, 0);
			//�����ʒu�ݒ�
			airShipPos_X = GetRand(50);

			isAirShipDirection = false;
		}
		MV1SetRotationXYZ(airShipHandle, airShipRotate);
	}
	//true�̎��E�i�s
	if (isAirShipDirection)
	{
		airShipPos_X += 0.1f;
	}
	//false�̎����i�s
	else
	{
		airShipPos_X -= 0.1f;
	}
	
	airShipPos = VGet(airShipPos_X, -30, airShipPos_Z);
	// 3D�̃|�W�V�����ݒ�
	MV1SetPosition(airShipHandle, airShipPos);
}

void BackGround::Draw()
{
	// �R�c���f���̕`��
	MV1DrawModel(startTileHandle);

	// �R�c���f���̕`��
	MV1DrawModel(homeHandle);

	// ���C�e�B���O�̌v�Z�����Ȃ��悤�ɐݒ��ύX
	SetUseLighting(FALSE);
	MV1DrawModel(airShipHandle);
	SetUseLighting(TRUE);

	MV1DrawModel(planeHandle);

	// �R�c���f���̕`��
	MV1DrawModel(castleHandle);

	// �R�c���f���̕`��
	MV1DrawModel(endTileHandle);
}