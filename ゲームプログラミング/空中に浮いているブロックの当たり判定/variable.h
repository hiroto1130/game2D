#pragma once
#ifndef VARIABLE_H_
#define VARIABLE_H_



////// �K�v��� //////

// �e�N�X�`�� //
const int D3DFVF_CUSTOMVERTEX(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
// �e�N�X�`����FVF���

// �L�[�{�[�h //
static const int MAX_KEY_NUMBER = 256;
// ���̓L�[������l
BYTE KeyState[MAX_KEY_NUMBER];
// 256�̔z�������
const int MASK_NUM = 0x80;
float GravityCount = 0;
float SpeedCount = 0;


#endif // !1

