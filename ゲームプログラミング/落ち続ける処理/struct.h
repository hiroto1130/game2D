#pragma once
#ifndef _STRUCT_H_
#define _STRUCT_H_

#include <Windows.h>

////////////////// �\���̐錾 ////////////////////

struct CUSTOMVERTEX
{
	float	// ���_���W
		x,  // ��
		y,  // ����
		z;  // ���s

	float   // ���Z��
		rhw;

	DWORD
		dwColor; // ���_�̐F

	float   // �e�N�X�`�����W
		tu,  // ��
		tv;	// ����
};

////////////////// �|���S���̍��W /////////////////

CUSTOMVERTEX v[4] =
{
{170.0f, 110.0f, 0.0f, 1.0f, 0xffffffff,0.0f,0.0f},// ���� 
{470.0f, 110.0f, 0.0f, 1.0f, 0xffffffff,1.0f,0.0f},// �E��
{470.0f, 410.0f, 0.0f, 1.0f, 0xffffffff,1.0f,1.0f},// �E��
{170.0f, 410.0f, 0.0f, 1.0f, 0xffffffff,0.0f,1.0f} // ����
};


#endif // !
