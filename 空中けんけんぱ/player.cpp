#include <math.h>
#include <vector>
#include "DxLib.h"
#include "Collision.h"
#include "Map.h"
#include "CommonFunction.h"
#include "GameStatus.h"
#include "WorldSprite.h"
#include"player.h"

Player::Player() 
{
    modelHandle = MV1LoadModel("material/Barbarian.mv1");   
    pos = VGet(0, 0, 0);

    //画像セット
    dashHandle[0] = LoadGraph("material/syutyu60.png");
    dashHandle[1] = LoadGraph("material/syutyu60_1.png");
    dashHandle[2] = LoadGraph("material/syutyu60_2.png");
    rightClickMouthHandle = LoadGraph("material/rightClickMouth_2.png");
    leftClickMouthHandle = LoadGraph("material/leftClickMouth_2.png");

    //サウンドセット
    jumpSound = LoadSoundMem("sound/se_jump.mp3");
    fallingSound = LoadSoundMem("sound/se_shot_005.mp3");
    speedUpSound = LoadSoundMem("sound/se_powerup10.mp3");
    fallOverSound = LoadSoundMem("sound/omanuke.mp3");

}

Player::~Player()
{
    // モデルのアンロード.
    MV1DeleteModel(modelHandle);
}

void Player::Initilize()
{
    pos = VGet(0, -10, 0);
    CameraPos = VGet(0, 0, 0);
    MV1SetScale(modelHandle, VGet(3.0f, 3.0f, 3.0f));


    // 3Dのポジション設定
    MV1SetPosition(modelHandle, pos);

    currentJumpSpeed = 0;	 // 現在のジャンプのスピード
    jumpPower = 1.2f;
    fastJumpPower = 0.9f;
    Gravity = 0.1f;
    NearJumpDistance = 6.0f / 24.0f;       //一度のジャンプで6ずつ進むようにする
    FarJumpDistance = 12.0f / 24.0f;
    fastNearJumpDistance = 6.0f / 18.0f;
    fastFarJumpDistance = 12.0f / 18.0f;
    motionNum = 10;
    col = 0;
    situation = 0;
    dashHandleCount = 0;
    flameCount = 0;
    timer = 200;
    commonFunction->TimerSet(startTime);

    jumpFlag = false;
    onGround = true;					 // 着地しているか？
    hitOnGround = true;				 // 着地しているか？
    touchFlg = false;
    fallingFlg = false;
    cameraFlg = false;
    JumpDisFlg = false;
    motionChangeFlg = false;
    timeFlg = false;
    isAcceleration = false;
    isGool = false;
    isfallingSound = false;
    isSpeedUpSound = false;
    isDisplay = false;

    // ３Ｄモデルの０番目のアニメーションをアタッチする
    ChangeMotion(motionNum);

    //右クリックの行き先を表示
    auto worldSprite = new WorldSprite();
    worldSprite->Initialize(rightClickMouthHandle, ChipPixelSize);
    rightChip.worldSprite = worldSprite;
    VECTOR rightChipPos = VGet(-6, -10, pos.z - 12.0f);
    rightChip.worldSprite->SetPos(rightChipPos);

    //左クリックの行き先を表示
    worldSprite->Initialize(leftClickMouthHandle, ChipPixelSize);
    leftChip.worldSprite = worldSprite;
    VECTOR leftChipPos = VGet(-6, -10, pos.z - 6.0f);
    leftChip.worldSprite->SetPos(leftChipPos);
 
}

void Player::Update(Map& map, CommonFunction& CommonFunction,int& gameStatus)
{

    if (GetMouseInput() & (MOUSE_INPUT_LEFT))
    {
        //ジャンプキーが押されてないかつ地面に足がついていればならジャンプする
        if (!jumpFlag)
        {
            if (onGround)
            {
                //フレーム20以内にジャンプすると加速する
                if (flameCount <= 4)
                {
                    isAcceleration = true;
                    if (!isSpeedUpSound)
                    {
                        //サウンド再生
                        PlaySoundMem(speedUpSound, DX_PLAYTYPE_BACK);
                        isSpeedUpSound = true;
                    }
                }

                //isAccelerationがfalseのとき通常のジャンプ力を加算する
                if (!isAcceleration)
                {
                    currentJumpSpeed += jumpPower;
                }
                else if (isAcceleration)
                {
                    currentJumpSpeed += fastJumpPower;
                }
                onGround = false;
                JumpDisFlg = false;
                flameCount = 0;
                //サウンド再生
                PlaySoundMem(jumpSound, DX_PLAYTYPE_BACK);
            }
        }
        jumpFlag = true;
    }
    else if (GetMouseInput() & (MOUSE_INPUT_RIGHT))
    {
        //ジャンプキーが押されてないかつ地面に足がついていればならジャンプする
        if (!jumpFlag)
        {
            if (onGround)
            {
                //フレーム20以内にジャンプすると加速する
                if (flameCount <= 4)
                {
                    isAcceleration = true;
                    if (!isSpeedUpSound)
                    {
                        //サウンド再生
                        PlaySoundMem(speedUpSound, DX_PLAYTYPE_BACK);
                        isSpeedUpSound = true;
                    }
                }

                //isAccelerationがfalseのとき通常のジャンプ力を加算する
                if (!isAcceleration)
                {
                    currentJumpSpeed += jumpPower;
                }
                else if (isAcceleration)
                {
                    currentJumpSpeed += fastJumpPower;
                }
                onGround = false;
                JumpDisFlg = true;
                flameCount = 0;
                //サウンド再生
                PlaySoundMem(jumpSound, DX_PLAYTYPE_BACK);
            }
        }
        jumpFlag = true;
    }
    else if(!motionChangeFlg)
    {
        jumpFlag = false;
    }

    //空中にいるときの処理
    if (!onGround)
    {
        //フラグがfalseのとき通常の速度でジャンプする
        if (!isAcceleration)
        {
            if (!JumpDisFlg)
            {
                ////////
                // モーションがジャンプしたら座標を移動させる
                ////////
                if (PlayTime >= 8.0f)
                {
                    pos.y += currentJumpSpeed;
                    currentJumpSpeed -= Gravity;
                    if (pos.y >  -10.0f)
                    {
                        pos.z -= NearJumpDistance;
                    }
                }
            }
            else
            {
                ////////
               // モーションがジャンプしたら座標を移動させる
               ////////
                if (PlayTime >= 8.0f)
                {
                    pos.y += currentJumpSpeed;
                    currentJumpSpeed -= Gravity;
                    if (pos.y > -10.0f)
                    {
                        pos.z -= FarJumpDistance;
                    }
                }
            }
        }
        //フラグがtrueのとき加速する
        else if (isAcceleration)
        {
            if (!JumpDisFlg)
            {
                ////////
                // モーションがジャンプしたら座標を移動させる
                ////////
                if (PlayTime >= 8.0f)
                {
                    pos.y += currentJumpSpeed;
                    currentJumpSpeed -= Gravity;
                    if (pos.y > -10.0f)
                    {
                        pos.z -= fastNearJumpDistance;
                    }
                }
            }
            else
            {
                ////////
               // モーションがジャンプしたら座標を移動させる
               ////////
                if (PlayTime >= 8.0f)
                {
                    pos.y += currentJumpSpeed;
                    currentJumpSpeed -= Gravity;
                    if (pos.y > -10.0f)
                    {
                        pos.z -= fastFarJumpDistance;
                    }
                }
            }
        }
    }

    if (!onGround)
    {
        if (pos.y < -20.0f)
        {
            if (!isfallingSound)
            {
                PlaySoundMem(fallingSound, DX_PLAYTYPE_BACK);
                isfallingSound = true;
            }
        }
      //-100あたりでplayerが戻ってくる位置を求め、カメラ移動を行う
        if (pos.y < -100.0f)
        {
            if (!fallingFlg)
            {
                int count = map.GetMapLayer();
                VECTOR returnPos[162];
                //一個前のmapChipの位置に戻す

                for (int i = 0; i < count; i++)
                {
                  returnPos[i] = ReturnToPos(map.ConstGetChip(i));
                }
                BackPos(returnPos, map);
                cameraFlg = true;
            }

            fallingFlg = true;

            //-350あたりでplayerを戻らせる
            if (pos.y < -350.0f)
            {
                cameraFlg = false;
            }

            if (!cameraFlg)
            {
                pos.z = CameraPos.z;
                onGround = true;
                fallingFlg = false;
            }
        }
        
        //着地確認
        if ((pos.y + currentJumpSpeed) < -10.0f && !fallingFlg)
        {
             CheckIsGround(map);
             //他のclass値を共有
             CommonFunction.SetSituation(situation);
             CommonFunction.Setcol(col);
            if (onGround)
            {
               currentJumpSpeed = 0.0f;

               pos.y = -10.0f;
               PlayTime = 0;
               flameCount = 0;
               isfallingSound = false; 
            }
        }
    }

    if (onGround && situation == 2)
    {
        CheckIsGround(map);
        //他のclass値を共有
        CommonFunction.SetSituation(situation);
        CommonFunction.Setcol(col);
    }
    
    if (onGround && situation == 3)
    {
        CheckIsGround(map);
        //他のclass値を共有
        CommonFunction.SetSituation(situation);
        CommonFunction.Setcol(col);
    }

    //10フレーム以内にジャンプすれば早くなる[
    if (onGround)
    {
        flameCount++;
    }
    if (flameCount >= 5)
    {
        flameCount = 5;
        isAcceleration = false;
        isSpeedUpSound = false;
    }

    //空中モーション
   
    if (!onGround)
    {
        // 再生時間を進める
        if (!isAcceleration)
        {
            PlayTime += 35.0f / 24.0f;
        }
        else if (isAcceleration)
        {
            PlayTime += 35.0f / 18.0f;
        }
    }

    //転んだ時のモーション
    if (motionChangeFlg)
    {
        PlayTime += 1.0f;

        // 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す
        if (PlayTime >= TotalTime && !timeFlg)
        {
            timeFlg = true;
            ChangeMotion(75);
        }
        else if (PlayTime >= TotalTime && timeFlg)
        {
            timeFlg = false;
            ChangeMotion(10);
            motionChangeFlg = false;
        }
    }

    //ゴールに着いたらリザルト
    if (col >= 160)
    {
        isGool = true;
    }

    // 再生時間をセットする
    MV1SetAttachAnimTime(modelHandle, AttachIndex, PlayTime);

    // 3Dのポジション設定
    MV1SetPosition(modelHandle, pos);

    if (isAcceleration)
    {
        if (!(GetNowCount() - startTime < timer))
        {
            dashHandleCount++;
            commonFunction->TimerSet(startTime);
        }

        if (dashHandleCount >= 3)
        {
            dashHandleCount = 0;
        }
    }

    //右クリックの行き先を表示
    VECTOR rightChipPos = VGet(-6, -10, pos.z - 12.0f);
    rightChip.worldSprite->SetPos(rightChipPos);

    //左クリックの行き先を表示
    VECTOR leftChipPos = VGet(-6, -10, pos.z - 6.0f);
    leftChip.worldSprite->SetPos(leftChipPos);
}

void Player::Draw()
{
    // ３Ｄモデルの描画
    MV1DrawModel(modelHandle);
    if (isAcceleration)
    {
        DrawGraph(0, 0, dashHandle[dashHandleCount], TRUE);
    }

  /*  rightChip.worldSprite->Draw();
    leftChip.worldSprite->Draw();*/
}

bool Player::IsHitWithMapChip(const VECTOR& futurePos, Map::Chip& mapChip)
{
    if (mapChip.vertex[0].y >= -10.0f)
    {

        // 当たっているかどうか調べる
        float futurePosLeft = futurePos.x;
        float futurePosRight = futurePos.x;
        float futurePosTop = -2.5f + futurePos.z;
        float futurePosBottom = 2.5f + futurePos.z;
        float targetLeft = mapChip.vertex[0].x;
        float targetRight = mapChip.vertex[1].x;
        float targetTop = mapChip.vertex[3].z;
        float targetBottom = mapChip.vertex[0].z;

    
        // 矩形同士の当たり判定
        if (((targetLeft <= futurePosLeft && futurePosLeft < targetRight) ||
            (targetLeft > futurePosLeft && targetLeft < futurePosRight)) &&
            ((targetTop <= futurePosTop && futurePosTop < targetBottom) ||
                (targetTop > futurePosTop && targetTop < futurePosBottom)))
        {
            //ギミックマスに止まった場合一定時間動けなくなる
            if (mapChip.situation == 1)
            {
                if (!motionChangeFlg)
                {
                    PlaySoundMem(fallOverSound, DX_PLAYTYPE_BACK);
                    ChangeMotion(72);
                    motionChangeFlg = true;
                    timeFlg = false;
                    jumpFlag = true;
                    mapChip.isHitGimmick = true; 
                }
            }
            else
            {
                if (motionChangeFlg)
                {
                    ChangeMotion(10);
                    motionChangeFlg = false;
                    timeFlg = false;
                }
            }
            //プレイヤーが何列目にいるかを特定
            col = mapChip.col;
            situation = mapChip.situation;
            return true;
        }
    }

    return false;
}

void Player::CheckIsGround(Map& map)
{
    bool isHit = false;
    int count = map.GetMapLayer();
    int**const count1 = map.GetMapLayer1();
   
    //足場に触れている場合true
    for (int i = 0; i < count; i++)
    {
        isHit = IsHitWithMapChip(pos, map.GetChip(i));

        if (isHit)
        {
            break;
        }
    }
    if (isHit)
    {
        onGround = true;
    }
    else
    {
        onGround = false;
    }
}

//マップチップのポジションを返す
VECTOR Player::ReturnToPos(const Map::Chip& mapChip)
{
    VECTOR ReturnPos;

    if (mapChip.col >= 0 && mapChip.situation != 2)
    {
        ReturnPos = mapChip.pos;
    }
    else
    {
        ReturnPos = VGet(0, 0, 0);
    }
  
    return ReturnPos;
}

/// <summary>
/// playerが戻ってくる位置を求める
/// </summary>
/// <param name="pos"></param>
/// <param name="map"></param>
void Player::BackPos(VECTOR pos[],Map& map)
{
    //足場にプレイヤーが存在するかどうか
    bool isHit = false;

    //プレイヤーの戻る量
    float backPos = this->pos.z + pos[0].z;

    //仮のプレイヤーの戻る量
    float provisionalBackPos;

    this->CameraPos = this->pos;

    //足場の数
    int count = map.GetMapLayer();

    while (true)
    {
        //////////////////////////////////////////
        // 現在の位置から最も近い前の足場に戻す
        /////////////////////////////////////////
        for (int i = 0; i < count; i++)
        {
            //現在のプレイヤーより奥の足場は計算しない
            if ((int)this->pos.z >= (int)pos[i].z)
            {
                break;
            }

            //現在のプレイヤーの位置からi番目の足場までの距離を計算
            provisionalBackPos = this->pos.z - pos[i].z;

            //仮の戻る量が現在の戻る量より大きい場合、置き換える
            if (backPos < provisionalBackPos)
            {
                backPos = provisionalBackPos;
            }
        }

        //playerが戻ってくる位置を保存する
        this->CameraPos.z = this->pos.z - backPos;

        //マップチップが存在する位置にいる場合whileループを抜ける
        for (int j = 0; j < count; j++)
        {
            //接地しているかどうか
            isHit = IsHitWithMapChip(this->CameraPos, map.GetChip(j));
            
            if (isHit)
            {
                break;
            }
        }
        
            break;
        
    }
    
}


void Player::ChangeMotion(int motionNum)
{
    MV1DetachAnim(modelHandle, AttachIndex);
    // 再生時間の初期化
     PlayTime = 0;
    // 再生時間をセットする
    MV1SetAttachAnimTime(modelHandle, AttachIndex, PlayTime);

    this->motionNum = motionNum;
    // ３Ｄモデルの０番目のアニメーションをアタッチする
    AttachIndex = MV1AttachAnim(modelHandle, this->motionNum, -1, FALSE);
    // アタッチしたアニメーションの総再生時間を取得する
    TotalTime = MV1GetAttachAnimTotalTime(modelHandle, AttachIndex);
}