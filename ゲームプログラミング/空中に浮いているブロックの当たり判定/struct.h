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

#endif // !