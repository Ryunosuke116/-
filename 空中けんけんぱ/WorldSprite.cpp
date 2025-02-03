#include "DxLib.h"
#include "WorldSprite.h"


void WorldSprite::Initialize(int chipGraph, int chipPixelSize,int spriteNo)
{
    ChangeFlg = false;
    swayFlg = false;
    VertexSize = 0;
    timer = 2000;
    this->textureGraph = chipGraph;

    int texW, texH;
    GetGraphTextureSize(textureGraph, &texW, &texH);
    int chipNum = texW / chipPixelSize;
    int chipNoX = spriteNo % chipNum;
    int chipNoY = spriteNo / chipNum;
    float oneChipUVRate = 1.0f / (float)chipNum;   // �e�N�X�`���S����1.0�Ƃ�������cihp��ɑ΂���uv�̃T�C�Y
   
    //u�͍���[�Av�͉E���[
    Vertex[0].u = (chipNoX + 0.0f) * oneChipUVRate;
    Vertex[0].v = (chipNoY + 0.0f) * oneChipUVRate;
    Vertex[1].u = (chipNoX + 1.0f) * oneChipUVRate;
    Vertex[1].v = (chipNoY + 0.0f) * oneChipUVRate;
    Vertex[2].u = (chipNoX + 0.0f) * oneChipUVRate;
    Vertex[2].v = (chipNoY + 1.0f) * oneChipUVRate;
    Vertex[3].u = (chipNoX + 1.0f) * oneChipUVRate;
    Vertex[3].v = (chipNoY + 1.0f) * oneChipUVRate;

   

    //�f�B�t���[�Y�A�X�y�L�����͏��������ɌŒ�
    for (int i = 0; i < 4; i++)
    {
        Vertex[i].dif = GetColorU8(255, 255, 255, 255);
        Vertex[i].spc = GetColorU8(0, 0, 0, 0);
        Vertex[i].norm = VGet(0.0f, 0.0f, -1.0f);    //��]���T�|�[�g���Ȃ��̂Ńm�[�}�����Œ�

       //  �⏕�e�N�X�`�����T�|�[�g���Ȃ��̂�uv�Œ�
        Vertex[i].su = 0.0f;
        Vertex[i].sv = 0.0f;
    }

   //�Q�|���S�����̃C���f�b�N�X�f�[�^���Z�b�g
    Index[0] = 0;
    Index[1] = 1;
    Index[2] = 2;
    Index[3] = 3;
    Index[4] = 2;   
    Index[5] = 1;

}

void WorldSprite::Initialize(int chipGraph, int chipPixelSize)
{
    this->textureGraph = chipGraph;

    int texX, texY;
    GetGraphTextureSize(textureGraph, &texX, &texY);
    int chipNum = texX;
    int chipNoX = texX;
    int chipNoY = texY;
    float oneChipUVRate = 1.0f;   // �e�N�X�`���S����1.0�Ƃ�������cihp��ɑ΂���uv�̃T�C�Y

    //u�͍���[�Av�͉E���[
    Vertex[0].u = 0;
    Vertex[0].v = 0;
    Vertex[1].u = 1;
    Vertex[1].v = 0;
    Vertex[2].u = 0;
    Vertex[2].v = 1;
    Vertex[3].u = 1;
    Vertex[3].v = 1;



    //�f�B�t���[�Y�A�X�y�L�����͏��������ɌŒ�
    for (int i = 0; i < 4; i++)
    {
        Vertex[i].dif = GetColorU8(255, 255, 255, 255);
        Vertex[i].spc = GetColorU8(0, 0, 0, 0);
        Vertex[i].norm = VGet(0.0f, 0.0f, -1.0f);    //��]���T�|�[�g���Ȃ��̂Ńm�[�}�����Œ�

        //  �⏕�e�N�X�`�����T�|�[�g���Ȃ��̂�uv�Œ�
        Vertex[i].su = 0.0f;
        Vertex[i].sv = 0.0f;
    }

    //�Q�|���S�����̃C���f�b�N�X�f�[�^���Z�b�g
    Index[0] = 0;
    Index[1] = 1;
    Index[2] = 2;
    Index[3] = 3;
    Index[4] = 2;
    Index[5] = 1;

}

void WorldSprite::SetPos(const VECTOR pos)
{
    //  �|�W�V����
    Vertex[0].pos = VAdd(pos, VGet(-2.5f, 0, 2.5f));
    Vertex[1].pos = VAdd(pos, VGet(2.5f, 0, 2.5f));
    Vertex[2].pos = VAdd(pos, VGet(-2.5f, 0, -2.5f));
    Vertex[3].pos = VAdd(pos, VGet(2.5f, 0, -2.5f));
}

void WorldSprite::getPos(VECTOR pos[])
{
    pos[0] = Vertex[0].pos;
    pos[1] = Vertex[1].pos;
    pos[2] = Vertex[2].pos;
    pos[3] = Vertex[3].pos;
}

void WorldSprite::Draw()
{
    // �Q�|���S���̕`��
    DrawPolygonIndexed3D(Vertex, 4, Index, 2, textureGraph, FALSE);
}

void WorldSprite::VertexSizeChangeSituation_2()
{

    // ���݌o�ߎ��Ԃ𓾂�
    //���Ԃ��o�߂���Ə������Ȃ�
    if (!(GetNowCount() - StartTime < timer))
    {
        //  false�̏ꍇ�A������Ȃ���
        if (!ChangeFlg)
        {
            Vertex[0].pos = VSub(Vertex[0].pos, VGet(-0.05f, 0.3f, 0.05f));
            Vertex[1].pos = VSub(Vertex[1].pos, VGet(0.05f, 0.3f, 0.05f));
            Vertex[2].pos = VSub(Vertex[2].pos, VGet(-0.05f, 0.3f, -0.05f));
            Vertex[3].pos = VSub(Vertex[3].pos, VGet(0.05f, 0.3f, -0.05f));
            VertexSize += 0.05f;

            if (VertexSize >= 2.5f)
            {
                ChangeFlg = true;
                TimerSet();
                timer = 1000;
            }
        }
        //true�̏ꍇ�A����𕜊�������
        else
        {
            Vertex[0].pos = VSub(Vertex[0].pos, VGet(0.05f, -0.3f, -0.05f));
            Vertex[1].pos = VSub(Vertex[1].pos, VGet(-0.05f, -0.3f, -0.05f));
            Vertex[2].pos = VSub(Vertex[2].pos, VGet(0.05f, -0.3f, 0.05f));
            Vertex[3].pos = VSub(Vertex[3].pos, VGet(-0.05f, -0.3f, 0.05f));
            VertexSize -= 0.05f;

            if (VertexSize <= 0)
            {
                ChangeFlg = false;
                TimerSet();
                timer = 2000;
            }
        }
    }
    if ((GetNowCount() - StartTime < timer))
    {
        if (!swayFlg && !ChangeFlg)
        {
            Vertex[0].pos = VSub(Vertex[0].pos, VGet(0.1f, 0, 0));
            Vertex[1].pos = VSub(Vertex[1].pos, VGet(0.1f, 0, 0));
            Vertex[2].pos = VSub(Vertex[2].pos, VGet(0.1f, 0, -0));
            Vertex[3].pos = VSub(Vertex[3].pos, VGet(0.1f, 0, -0));
            swayFlg = true;
        }
        else if(swayFlg && !ChangeFlg)
        {
            Vertex[0].pos = VSub(Vertex[0].pos, VGet(-0.1f, 0, 0));
            Vertex[1].pos = VSub(Vertex[1].pos, VGet(-0.1f, 0, 0));
            Vertex[2].pos = VSub(Vertex[2].pos, VGet(-0.1f, 0, -0));
            Vertex[3].pos = VSub(Vertex[3].pos, VGet(-0.1f, 0, -0));
            swayFlg = false;
        }

    }
}

void WorldSprite::VertexSizeChangeSituation_3()
{
    if (VertexSize <= 2.5f)
    {
        Vertex[0].pos = VSub(Vertex[0].pos, VGet(-0.05f, 0.3f, 0.05f));
        Vertex[1].pos = VSub(Vertex[1].pos, VGet(0.05f, 0.3f, 0.05f));
        Vertex[2].pos = VSub(Vertex[2].pos, VGet(-0.05f, 0.3f, -0.05f));
        Vertex[3].pos = VSub(Vertex[3].pos, VGet(0.05f, 0.3f, -0.05f));
        VertexSize += 0.05f;
    }
}

void WorldSprite::TimerSet()
{
    StartTime = GetNowCount();
}

void WorldSprite::VertexSizeReset()
{
    VertexSize = 0;
}