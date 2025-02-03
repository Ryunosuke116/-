#pragma once
class Collision
{
private:
		VECTOR pos;
		float w = 0;		// 当たり矩形幅ピクセル数
		float h = 0;        // 当たり矩形高さピクセル数
		float worldLX = 0;  // 左上X座標
		float worldLY = 0;  // 左上Y座標
		float worldRX = 0;  // 右下X座標
		float worldRY = 0;  // 右下Y座標
public:
	bool isHitRect(const Collision& rect1, const Collision& rect2);

	void initRect(VECTOR pos, Collision& dstRect, float w, float h);

	void drawRect(Collision& dstRect, int scrollX);

	void updateWorldRect(Collision& dstRect,VECTOR pos, float x, float y);

};

