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
{  0.0f,   0.0f, 0.0f, 1.0f, 0xffffffff,0.0f,0.0f},// ���� 
{100.0f,   0.0f, 0.0f, 1.0f, 0xffffffff,1.0f,0.0f},// �E��
{100.0f, 100.0f, 0.0f, 1.0f, 0xffffffff,1.0f,1.0f},// �E��
{  0.0f, 100.0f, 0.0f, 1.0f, 0xffffffff,0.0f,1.0f} // ����
};

CUSTOMVERTEX v2[4] =
{
{  0.0f, 341.0f, 0.0f, 1.0f, 0xffffffff,0.0f,0.0f},// ���� 
{250.0f, 341.0f, 0.0f, 1.0f, 0xffffffff,1.0f,0.0f},// �E��
{250.0f, 441.0f, 0.0f, 1.0f, 0xffffffff,1.0f,1.0f},// �E��
{  0.0f, 441.0f, 0.0f, 1.0f, 0xffffffff,0.0f,1.0f} // ����
};


#endif // !