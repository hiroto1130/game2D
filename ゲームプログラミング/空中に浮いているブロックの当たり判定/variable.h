#pragma once
#ifndef VARIABLE_H_
#define VARIABLE_H_



////// 必要情報 //////

// テクスチャ //
const int D3DFVF_CUSTOMVERTEX(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
// テクスチャのFVF情報

// キーボード //
static const int MAX_KEY_NUMBER = 256;
// 入力キーを入れる値
BYTE KeyState[MAX_KEY_NUMBER];
// 256の配列を持つ
const int MASK_NUM = 0x80;
float GravityCount = 0;
float SpeedCount = 0;


#endif // !1

