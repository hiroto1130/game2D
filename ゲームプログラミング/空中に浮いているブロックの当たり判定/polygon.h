#pragma once
#ifndef POLYGON_H_
#define POLYGON_H_

#include"struct.h"

////////////////// ポリゴンの座標 /////////////////

CUSTOMVERTEX v[4] =
{
{  0.0f,   0.0f, 0.0f, 1.0f, 0xffffffff,0.0f,0.0f},// 左上 
{ 50.0f,   0.0f, 0.0f, 1.0f, 0xffffffff,1.0f,0.0f},// 右上
{ 50.0f,  50.0f, 0.0f, 1.0f, 0xffffffff,1.0f,1.0f},// 右下
{  0.0f,  50.0f, 0.0f, 1.0f, 0xffffffff,0.0f,1.0f} // 左下
};

CUSTOMVERTEX v2[4] =
{
{  0.0f, 341.0f, 0.0f, 1.0f, 0xffffffff,0.0f,0.0f},// 左上 
{255.0f, 341.0f, 0.0f, 1.0f, 0xffffffff,1.0f,0.0f},// 右上
{255.0f, 441.0f, 0.0f, 1.0f, 0xffffffff,1.0f,1.0f},// 右下
{  0.0f, 441.0f, 0.0f, 1.0f, 0xffffffff,0.0f,1.0f} // 左下
};

CUSTOMVERTEX v3[4] =
{
{350.0f, 200.0f, 0.0f, 1.0f, 0xffffffff,0.0f,0.0f},// 左上 
{625.0f, 200.0f, 0.0f, 1.0f, 0xffffffff,1.0f,0.0f},// 右上
{625.0f, 270.0f, 0.0f, 1.0f, 0xffffffff,1.0f,1.0f},// 右下
{350.0f, 270.0f, 0.0f, 1.0f, 0xffffffff,0.0f,1.0f} // 左下
};

#endif // !


