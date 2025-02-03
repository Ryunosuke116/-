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

    //�摜�Z�b�g
    dashHandle[0] = LoadGraph("material/syutyu60.png");
    dashHandle[1] = LoadGraph("material/syutyu60_1.png");
    dashHandle[2] = LoadGraph("material/syutyu60_2.png");
    rightClickMouthHandle = LoadGraph("material/rightClickMouth_2.png");
    leftClickMouthHandle = LoadGraph("material/leftClickMouth_2.png");

    //�T�E���h�Z�b�g
    jumpSound = LoadSoundMem("sound/se_jump.mp3");
    fallingSound = LoadSoundMem("sound/se_shot_005.mp3");
    speedUpSound = LoadSoundMem("sound/se_powerup10.mp3");
    fallOverSound = LoadSoundMem("sound/omanuke.mp3");

}

Player::~Player()
{
    // ���f���̃A�����[�h.
    MV1DeleteModel(modelHandle);
}

void Player::Initilize()
{
    pos = VGet(0, -10, 0);
    CameraPos = VGet(0, 0, 0);
    MV1SetScale(modelHandle, VGet(3.0f, 3.0f, 3.0f));


    // 3D�̃|�W�V�����ݒ�
    MV1SetPosition(modelHandle, pos);

    currentJumpSpeed = 0;	 // ���݂̃W�����v�̃X�s�[�h
    jumpPower = 1.2f;
    fastJumpPower = 0.9f;
    Gravity = 0.1f;
    NearJumpDistance = 6.0f / 24.0f;       //��x�̃W�����v��6���i�ނ悤�ɂ���
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
    onGround = true;					 // ���n���Ă��邩�H
    hitOnGround = true;				 // ���n���Ă��邩�H
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

    // �R�c���f���̂O�Ԗڂ̃A�j���[�V�������A�^�b�`����
    ChangeMotion(motionNum);

    //�E�N���b�N�̍s�����\��
    auto worldSprite = new WorldSprite();
    worldSprite->Initialize(rightClickMouthHandle, ChipPixelSize);
    rightChip.worldSprite = worldSprite;
    VECTOR rightChipPos = VGet(-6, -10, pos.z - 12.0f);
    rightChip.worldSprite->SetPos(rightChipPos);

    //���N���b�N�̍s�����\��
    worldSprite->Initialize(leftClickMouthHandle, ChipPixelSize);
    leftChip.worldSprite = worldSprite;
    VECTOR leftChipPos = VGet(-6, -10, pos.z - 6.0f);
    leftChip.worldSprite->SetPos(leftChipPos);
 
}

void Player::Update(Map& map, CommonFunction& CommonFunction,int& gameStatus)
{

    if (GetMouseInput() & (MOUSE_INPUT_LEFT))
    {
        //�W�����v�L�[��������ĂȂ����n�ʂɑ������Ă���΂Ȃ�W�����v����
        if (!jumpFlag)
        {
            if (onGround)
            {
                //�t���[��20�ȓ��ɃW�����v����Ɖ�������
                if (flameCount <= 4)
                {
                    isAcceleration = true;
                    if (!isSpeedUpSound)
                    {
                        //�T�E���h�Đ�
                        PlaySoundMem(speedUpSound, DX_PLAYTYPE_BACK);
                        isSpeedUpSound = true;
                    }
                }

                //isAcceleration��false�̂Ƃ��ʏ�̃W�����v�͂����Z����
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
                //�T�E���h�Đ�
                PlaySoundMem(jumpSound, DX_PLAYTYPE_BACK);
            }
        }
        jumpFlag = true;
    }
    else if (GetMouseInput() & (MOUSE_INPUT_RIGHT))
    {
        //�W�����v�L�[��������ĂȂ����n�ʂɑ������Ă���΂Ȃ�W�����v����
        if (!jumpFlag)
        {
            if (onGround)
            {
                //�t���[��20�ȓ��ɃW�����v����Ɖ�������
                if (flameCount <= 4)
                {
                    isAcceleration = true;
                    if (!isSpeedUpSound)
                    {
                        //�T�E���h�Đ�
                        PlaySoundMem(speedUpSound, DX_PLAYTYPE_BACK);
                        isSpeedUpSound = true;
                    }
                }

                //isAcceleration��false�̂Ƃ��ʏ�̃W�����v�͂����Z����
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
                //�T�E���h�Đ�
                PlaySoundMem(jumpSound, DX_PLAYTYPE_BACK);
            }
        }
        jumpFlag = true;
    }
    else if(!motionChangeFlg)
    {
        jumpFlag = false;
    }

    //�󒆂ɂ���Ƃ��̏���
    if (!onGround)
    {
        //�t���O��false�̂Ƃ��ʏ�̑��x�ŃW�����v����
        if (!isAcceleration)
        {
            if (!JumpDisFlg)
            {
                ////////
                // ���[�V�������W�����v��������W���ړ�������
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
               // ���[�V�������W�����v��������W���ړ�������
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
        //�t���O��true�̂Ƃ���������
        else if (isAcceleration)
        {
            if (!JumpDisFlg)
            {
                ////////
                // ���[�V�������W�����v��������W���ړ�������
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
               // ���[�V�������W�����v��������W���ړ�������
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
      //-100�������player���߂��Ă���ʒu�����߁A�J�����ړ����s��
        if (pos.y < -100.0f)
        {
            if (!fallingFlg)
            {
                int count = map.GetMapLayer();
                VECTOR returnPos[162];
                //��O��mapChip�̈ʒu�ɖ߂�

                for (int i = 0; i < count; i++)
                {
                  returnPos[i] = ReturnToPos(map.ConstGetChip(i));
                }
                BackPos(returnPos, map);
                cameraFlg = true;
            }

            fallingFlg = true;

            //-350�������player��߂点��
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
        
        //���n�m�F
        if ((pos.y + currentJumpSpeed) < -10.0f && !fallingFlg)
        {
             CheckIsGround(map);
             //����class�l�����L
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
        //����class�l�����L
        CommonFunction.SetSituation(situation);
        CommonFunction.Setcol(col);
    }
    
    if (onGround && situation == 3)
    {
        CheckIsGround(map);
        //����class�l�����L
        CommonFunction.SetSituation(situation);
        CommonFunction.Setcol(col);
    }

    //10�t���[���ȓ��ɃW�����v����Α����Ȃ�[
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

    //�󒆃��[�V����
   
    if (!onGround)
    {
        // �Đ����Ԃ�i�߂�
        if (!isAcceleration)
        {
            PlayTime += 35.0f / 24.0f;
        }
        else if (isAcceleration)
        {
            PlayTime += 35.0f / 18.0f;
        }
    }

    //�]�񂾎��̃��[�V����
    if (motionChangeFlg)
    {
        PlayTime += 1.0f;

        // �Đ����Ԃ��A�j���[�V�����̑��Đ����ԂɒB������Đ����Ԃ��O�ɖ߂�
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

    //�S�[���ɒ������烊�U���g
    if (col >= 160)
    {
        isGool = true;
    }

    // �Đ����Ԃ��Z�b�g����
    MV1SetAttachAnimTime(modelHandle, AttachIndex, PlayTime);

    // 3D�̃|�W�V�����ݒ�
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

    //�E�N���b�N�̍s�����\��
    VECTOR rightChipPos = VGet(-6, -10, pos.z - 12.0f);
    rightChip.worldSprite->SetPos(rightChipPos);

    //���N���b�N�̍s�����\��
    VECTOR leftChipPos = VGet(-6, -10, pos.z - 6.0f);
    leftChip.worldSprite->SetPos(leftChipPos);
}

void Player::Draw()
{
    // �R�c���f���̕`��
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

        // �������Ă��邩�ǂ������ׂ�
        float futurePosLeft = futurePos.x;
        float futurePosRight = futurePos.x;
        float futurePosTop = -2.5f + futurePos.z;
        float futurePosBottom = 2.5f + futurePos.z;
        float targetLeft = mapChip.vertex[0].x;
        float targetRight = mapChip.vertex[1].x;
        float targetTop = mapChip.vertex[3].z;
        float targetBottom = mapChip.vertex[0].z;

    
        // ��`���m�̓����蔻��
        if (((targetLeft <= futurePosLeft && futurePosLeft < targetRight) ||
            (targetLeft > futurePosLeft && targetLeft < futurePosRight)) &&
            ((targetTop <= futurePosTop && futurePosTop < targetBottom) ||
                (targetTop > futurePosTop && targetTop < futurePosBottom)))
        {
            //�M�~�b�N�}�X�Ɏ~�܂����ꍇ��莞�ԓ����Ȃ��Ȃ�
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
            //�v���C���[������ڂɂ��邩�����
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
   
    //����ɐG��Ă���ꍇtrue
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

//�}�b�v�`�b�v�̃|�W�V������Ԃ�
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
/// player���߂��Ă���ʒu�����߂�
/// </summary>
/// <param name="pos"></param>
/// <param name="map"></param>
void Player::BackPos(VECTOR pos[],Map& map)
{
    //����Ƀv���C���[�����݂��邩�ǂ���
    bool isHit = false;

    //�v���C���[�̖߂��
    float backPos = this->pos.z + pos[0].z;

    //���̃v���C���[�̖߂��
    float provisionalBackPos;

    this->CameraPos = this->pos;

    //����̐�
    int count = map.GetMapLayer();

    while (true)
    {
        //////////////////////////////////////////
        // ���݂̈ʒu����ł��߂��O�̑���ɖ߂�
        /////////////////////////////////////////
        for (int i = 0; i < count; i++)
        {
            //���݂̃v���C���[��艜�̑���͌v�Z���Ȃ�
            if ((int)this->pos.z >= (int)pos[i].z)
            {
                break;
            }

            //���݂̃v���C���[�̈ʒu����i�Ԗڂ̑���܂ł̋������v�Z
            provisionalBackPos = this->pos.z - pos[i].z;

            //���̖߂�ʂ����݂̖߂�ʂ��傫���ꍇ�A�u��������
            if (backPos < provisionalBackPos)
            {
                backPos = provisionalBackPos;
            }
        }

        //player���߂��Ă���ʒu��ۑ�����
        this->CameraPos.z = this->pos.z - backPos;

        //�}�b�v�`�b�v�����݂���ʒu�ɂ���ꍇwhile���[�v�𔲂���
        for (int j = 0; j < count; j++)
        {
            //�ڒn���Ă��邩�ǂ���
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
    // �Đ����Ԃ̏�����
     PlayTime = 0;
    // �Đ����Ԃ��Z�b�g����
    MV1SetAttachAnimTime(modelHandle, AttachIndex, PlayTime);

    this->motionNum = motionNum;
    // �R�c���f���̂O�Ԗڂ̃A�j���[�V�������A�^�b�`����
    AttachIndex = MV1AttachAnim(modelHandle, this->motionNum, -1, FALSE);
    // �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
    TotalTime = MV1GetAttachAnimTotalTime(modelHandle, AttachIndex);
}