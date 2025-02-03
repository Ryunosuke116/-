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

	bool isAirShipDirection;	//どっち向きか
	bool isPlaneDirection;
	
	VECTOR startTilePos;         //ポジション
	VECTOR endTilePos;
	VECTOR castlePos;
	VECTOR homePos;
	VECTOR airShipPos;
	VECTOR planePos;
	//モデルの向き調整
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

