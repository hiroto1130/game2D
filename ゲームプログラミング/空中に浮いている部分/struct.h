#pragma once
#pragma once
#ifndef _STRUCT_H_
#define _STRUCT_H_

#include <Windows.h>

////////////////// 構造体宣言 ////////////////////

struct CUSTOMVERTEX
{
	float	// 頂点座標
		x,  // 幅
		y,  // 高さ
		z;  // 奥行

	float   // 除算数
		rhw;

	DWORD
		dwColor; // 頂点の色

	float   // テクスチャ座標
		tu,  // 幅
		tv;	// 高さ
};

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
{ 200.0f, 200.0f, 0.0f, 1.0f, 0xffffffff,0.0f,0.0f},// 左上 
{ 300.0f, 200.0f, 0.0f, 1.0f, 0xffffffff,1.0f,0.0f},// 右上
{ 300.0f, 300.0f, 0.0f, 1.0f, 0xffffffff,1.0f,1.0f},// 右下
{ 200.0f, 300.0f, 0.0f, 1.0f, 0xffffffff,0.0f,1.0f} // 左下
};


#endif // !