#include "DxLib.h"
#include <math.h>
#include <vector>
#include "Map.h"
#include "Collision.h"
#include "WorldSprite.h"
#include "Loader.h"
#include"CommonFunction.h"


Map::Map()
{
	mapChipHandle = LoadGraph("material/Island2.png");
	skyHandle = LoadGraph("material/sky.png");
	pos = VGet(0, 0, 0);
	Rotate = VGet(1.7f, 0, 0);
	loader = new Loader();
}

Map::~Map()
{
	for (auto& chip : chips)
	{
		if (chip.worldSprite != nullptr)
		{
			if (chip.col != -1)
			{
				delete chip.worldSprite;
				chip.worldSprite = nullptr;
				if (chip.gimmickGraph > -1)
				{
					MV1DeleteModel(chip.gimmickGraph);
				}
			}
		}
	}
	chips.clear();
	if (loader != nullptr)
	{
		delete loader;
		loader = nullptr;
	}


}

void Map::Load()
{

	loader->mapLayerLoader(mapLayer.mapData, mapLayer.mapXNum,
							mapLayer.mapYNum, "material/map.csv");
	for (int i = 0; i < mapLayer.mapYNum; i++)
	{
		for (int j = 0; j < mapLayer.mapXNum; j++)
		{
			if (mapLayer.mapData[i][j] > 0)
			{
				auto worldSprite = new WorldSprite();
				worldSprite->Initialize(mapChipHandle, ChipPixelSize,mapLayer.mapData[i][j]);

				Chip chip;
				if (mapLayer.mapData[i][j] == 37)
				{
					chip.situation = 1;
					chip.isHitGimmick = false;
					//カエルをセット
					chip.gimmickGraph = MV1LoadModel("material/untitled.mv1");
					MV1SetScale(chip.gimmickGraph, VGet(0.01f, 0.01f, 0.01f));
					MV1SetRotationXYZ(chip.gimmickGraph, VGet(0, -4.7, 0));
				}
				else if (mapLayer.mapData[i][j] == 4)
				{
					chip.situation = 2;
					chip.resetTimerFlg = false;
				}
				else if (mapLayer.mapData[i][j] == 2)
				{
					chip.situation = 3;
				}
				else
				{
					chip.situation = 0;
				}

				chip.worldSprite = worldSprite;
				VECTOR chipPos = VGet(0,-10, i * -6.0f);
				worldSprite->SetPos(chipPos);
				worldSprite->getPos(chip.vertex);
				chip.pos = chipPos;

				chip.col = i;
				chips.push_back(chip);
			}
			else
			{
				Chip chip;
				chip.col = -1;
				VECTOR chipPos = VGet(0, -10, i * -6.0f);
				chip.pos = chipPos;
				chips.push_back(chip);
			}
		}
	}
}

void Map::Update(CommonFunction& CommonFunction)
{
	if (backGroundX <= 1600)
	{
		backGroundX += 1;
	}
	else
	{
		backGroundX = 0;
	}
	int count = GetMapLayer();
	for (int i = 0; i < count; i++)
	{
		MapChipSizeChange(GetChip(i),CommonFunction);
	}
}

void Map::Draw()
{
	MV1DrawModel(backGroundHandle);
	DrawGraph(backGroundX, 0, skyHandle, false);
	DrawGraph(backGroundX - 1600, 0, skyHandle, false);

	for (const auto& chip : chips)
	{
		if (chip.col != -1)
		{
			chip.worldSprite->Draw();
		}
		if (chip.situation == 1 && !chip.isHitGimmick)
		{
			// 3Dのポジション設定
 			MV1SetPosition(chip.gimmickGraph,VGet(chip.pos.x, -9.0f, chip.pos.z));
			MV1DrawModel(chip.gimmickGraph);
		}
	}
}

void Map::ResultDraw()
{
	DrawGraph(backGroundX, 0, skyHandle, false);
	DrawGraph(backGroundX - 1600, 0, skyHandle, false);
}

const Map::Chip& Map::ConstGetChip(int col) const
{
	for (const auto& chip : chips)
	{
		if (col == chip.col)
		{
			return chip;
		}
	}
}

Map::Chip& Map::GetChip(int col)
{
	for (auto& chip : chips)
	{
		if (col == chip.col)
		{
			return chip;
		}
	}
}

//situationが2の場合足場を出たり消したりする
void Map::MapChipSizeChange(Chip& mapChip, CommonFunction& CommonFunction)
{
	//playerが何列目にいるか
	const int col = CommonFunction.GetCol();
	const int situation = CommonFunction.GetSituation();

	if (mapChip.situation == 2)
	{
		if (!mapChip.resetTimerFlg)
		{
			mapChip.worldSprite->TimerSet();
			mapChip.resetTimerFlg = true;
		}
		mapChip.worldSprite->VertexSizeChangeSituation_2();
		mapChip.worldSprite->getPos(mapChip.vertex);
	}
	if (mapChip.situation == 3 && col == mapChip.col)
	{
		mapChip.worldSprite->VertexSizeChangeSituation_3();
		mapChip.worldSprite->getPos(mapChip.vertex);
	}
}

/// <summary>
/// マップチップをリセット
/// </summary>
void Map::MapChipReset(Chip& mapChip, const int col)
{
	if (mapChip.situation == 1)
	{
		mapChip.isHitGimmick = false;
	}

	if (mapChip.situation == 3)
	{
		VECTOR chipPos = VGet(0, -10, col * -6.0f);
		mapChip.worldSprite->SetPos(chipPos);
		mapChip.worldSprite->getPos(mapChip.vertex);
		mapChip.worldSprite->VertexSizeReset();
		chip.pos = chipPos;
	}
	
}