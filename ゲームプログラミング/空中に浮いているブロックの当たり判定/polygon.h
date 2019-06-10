#pragma once
#ifndef POLYGON_H_
#define POLYGON_H_

#include"struct.h"

////////////////// �|���S���̍��W /////////////////

CUSTOMVERTEX v[4] =
{
{  0.0f,   0.0f, 0.0f, 1.0f, 0xffffffff,0.0f,0.0f},// ���� 
{ 50.0f,   0.0f, 0.0f, 1.0f, 0xffffffff,1.0f,0.0f},// �E��
{ 50.0f,  50.0f, 0.0f, 1.0f, 0xffffffff,1.0f,1.0f},// �E��
{  0.0f,  50.0f, 0.0f, 1.0f, 0xffffffff,0.0f,1.0f} // ����
};

CUSTOMVERTEX v2[4] =
{
{  0.0f, 341.0f, 0.0f, 1.0f, 0xffffffff,0.0f,0.0f},// ���� 
{255.0f, 341.0f, 0.0f, 1.0f, 0xffffffff,1.0f,0.0f},// �E��
{255.0f, 441.0f, 0.0f, 1.0f, 0xffffffff,1.0f,1.0f},// �E��
{  0.0f, 441.0f, 0.0f, 1.0f, 0xffffffff,0.0f,1.0f} // ����
};

CUSTOMVERTEX v3[4] =
{
{350.0f, 200.0f, 0.0f, 1.0f, 0xffffffff,0.0f,0.0f},// ���� 
{625.0f, 200.0f, 0.0f, 1.0f, 0xffffffff,1.0f,0.0f},// �E��
{625.0f, 270.0f, 0.0f, 1.0f, 0xffffffff,1.0f,1.0f},// �E��
{350.0f, 270.0f, 0.0f, 1.0f, 0xffffffff,0.0f,1.0f} // ����
};

#endif // !


