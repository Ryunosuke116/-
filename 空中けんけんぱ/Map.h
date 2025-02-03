#pragma once

class WorldSprite;
class Loader;
class CommonFunction;

class Map
{
public:

	Map();
	~Map();

	struct Chip
	{
		VECTOR pos;
		WorldSprite* worldSprite;
		int col;					//��
		int situation;				//�M�~�b�N�U�蕪��
		int gimmickGraph;
		VECTOR vertex[4];
		bool resetTimerFlg;
		bool isHitGimmick;
	};
	Chip chip;
	struct sMapLayer
	{
		int** mapData; // �}�b�v�`�b�v��2�����z��
		int   mapXNum; // �}�b�v�`�b�v���������ɕ���ł��鐔
		int   mapYNum; // �}�b�v�`�b�v���c�����ɕ���ł��鐔
	};
	sMapLayer mapLayer;

	void MapChipSizeChange(Chip& mapChip, CommonFunction& CommonFunction);
	void Load();
	void Update(CommonFunction& CommonFunction);
	void Draw();
	void ResultDraw();
	void MapChipReset(Chip& mapChip, const int col);
	const int& GetMapLayer()const { return mapLayer.mapYNum; }
	int** GetMapLayer1()const { return  mapLayer.mapData; }
	const int& GetSituation()const { return chip.situation; }
	const int& GetHandle()const { return mapChipHandle; }
	const int& GetChips()const { return chips.capacity(); }
	const Chip& ConstGetChip(int col)const;
	Chip& GetChip(int col);

	// �}�b�v�`�b�v�̃T�C�Y
	static constexpr float	ChipSize = 0.725f;
	static constexpr int	ChipPixelSize = 32;

private:
	VECTOR Rotate;
	VECTOR pos;
	int mapChipHandle;
	int backGroundHandle;
	int skyHandle;
	std::vector<Chip> chips;
	Loader* loader;
	int backGroundX;

};

