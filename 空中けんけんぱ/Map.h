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
		int col;					//列
		int situation;				//ギミック振り分け
		int gimmickGraph;
		VECTOR vertex[4];
		bool resetTimerFlg;
		bool isHitGimmick;
	};
	Chip chip;
	struct sMapLayer
	{
		int** mapData; // マップチップの2次元配列
		int   mapXNum; // マップチップが横方向に並んでいる数
		int   mapYNum; // マップチップが縦方向に並んでいる数
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

	// マップチップのサイズ
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

