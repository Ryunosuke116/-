#pragma once
#include "DxLib.h"
#include "Collision.h"

class WorldSprite
{
private:
	VECTOR		pos;			// 描画ポジション
	VERTEX3D	Vertex[4];		// 頂点バッファ
	WORD		Index[6];		// インデックスバッファ
	int			textureGraph;	// スプライトを作成する元テクスチャのグラフィックハンドル
	int			StartTime;
	int			timer;
	bool		ChangeFlg;
	bool		swayFlg;
	float		VertexSize;
public:

	void Initialize(int chipGraph, int chipPixelSize,int spriteNo);
	void Initialize(int chipGraph, int chipPixelSize);
	void SetPos(const VECTOR pos);
	void Draw();

	void getPos(VECTOR pos[]);
	void VertexSizeChangeSituation_2();
	void VertexSizeChangeSituation_3();
	void TimerSet();
	void VertexSizeReset();
};

