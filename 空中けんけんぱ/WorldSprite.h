#pragma once
#include "DxLib.h"
#include "Collision.h"

class WorldSprite
{
private:
	VECTOR		pos;			// �`��|�W�V����
	VERTEX3D	Vertex[4];		// ���_�o�b�t�@
	WORD		Index[6];		// �C���f�b�N�X�o�b�t�@
	int			textureGraph;	// �X�v���C�g���쐬���錳�e�N�X�`���̃O���t�B�b�N�n���h��
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

