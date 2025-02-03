#pragma once
#include "Collision.h"
#include "Map.h"

class Collision;
class Map;
class CommonFunction;

class Player 
{
private:
    int modelHandle;    //モデルハンドル
    int rightClickMouthHandle;
    int leftClickMouthHandle;
    int dashHandle[3];
    int dashHandleCount;
    int AttachIndex;
    int motionNum;
    int col;
    int situation;
    int timer;
    int startTime;
    int flameCount;
    int jumpSound;
    int fallingSound;
    int speedUpSound;
    int fallOverSound;
    VECTOR pos;         //ポジション
    VECTOR CameraPos;
    bool jumpFlag;
    bool onGround;					 // 着地しているか？
    bool hitOnGround;				 // 着地しているか？
    bool touchFlg;
    bool fallingFlg;
    bool JumpDisFlg;                //遠くに飛ぶか近くに飛ぶか
    bool motionChangeFlg;           //モーションを切り替える
    bool timeFlg;
    bool isAcceleration;
    bool isGool;
    bool isfallingSound;
    bool isSpeedUpSound;
    bool isDisplay;

    float moveSpeed;
    float currentJumpSpeed;	 // 現在のジャンプのスピード
    float jumpPower;
    float fastJumpPower;
    float Gravity;
    float NearJumpDistance;
    float FarJumpDistance;
    float fastNearJumpDistance;
    float fastFarJumpDistance;
    float TotalTime;
    float PlayTime;

    bool IsHitWithMapChip(const VECTOR& futurePos,Map::Chip& mapChip);

    // マップチップのサイズ
    static constexpr float	ChipSize = 0.725f;
    static constexpr int	ChipPixelSize = 32;


public:
    Player();
    ~Player();

    void Initilize();
    void Update(Map& map,CommonFunction& CommonFunction, int& gameStatus);
    void Draw();
    void CheckIsGround(Map& map);
    void BackPos(VECTOR pos[],Map& map);
    void ChangeMotion(int motionNum);
    const VECTOR& GetPos() { return pos; }
    const VECTOR& GetCameraPos() { return CameraPos; }
    const bool& GetIsAcceleration() { return isAcceleration; }
    const bool& GetIsGool() { return isGool; }
    const int& GetCol() const { return col; }
    VECTOR ReturnToPos(const Map::Chip& mapChip);
    CommonFunction* commonFunction;
    bool cameraFlg;

    struct Chip
    {
        VECTOR pos;
        WorldSprite* worldSprite;
        VECTOR vertex[4];
    };

    Chip rightChip;
    Chip leftChip;
};