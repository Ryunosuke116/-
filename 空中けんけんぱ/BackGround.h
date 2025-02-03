#pragma once
class BackGround
{
private:
	int startTileHandle;
	int endTileHandle;
	int castleHandle;
	int homeHandle;
	int airShipHandle;
	int planeHandle;
	int airShipAppearanceCount;
	int planeAppearanceCount;

	float airShipPos_X;
	float airShipPos_Z;
	float planePos_X;
	float planePos_Z;

	bool isAirShipDirection;	//�ǂ���������
	bool isPlaneDirection;
	
	VECTOR startTilePos;         //�|�W�V����
	VECTOR endTilePos;
	VECTOR castlePos;
	VECTOR homePos;
	VECTOR airShipPos;
	VECTOR planePos;
	//���f���̌�������
	VECTOR rotate;
	VECTOR airShipRotate;
	VECTOR planeRotata;
public:
	BackGround();
	~BackGround();

	void Initilize();
	void Update(const Player& player);
	void Draw();
};

