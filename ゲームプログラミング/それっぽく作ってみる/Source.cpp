#include <windows.h>
#include<d3dx9.h>
#include<tchar.h>
#include<dinput.h>

// ウィンドウの大きさ //
#define WND_WIDTH 700 // 横 X
#define WND_HEIGHT 830　// 縦　Y

/////// ポインタ定義 ////// 

LPDIRECT3DTEXTURE9 pTexture;
LPDIRECT3DTEXTURE9 pTexture2;
LPDIRECT3DTEXTURE9 pTexture3;
LPDIRECT3DTEXTURE9 pTexture4;
LPDIRECT3DTEXTURE9 pTexture5;
LPDIRECT3DTEXTURE9 pTexture6;

LPDIRECT3D9 pDirect3d;
LPDIRECT3DDEVICE9 pDevice;
LPDIRECTINPUT8 pDinput = NULL;
LPDIRECTINPUTDEVICE8 pkey = NULL;

////// プロトタイプ宣言 //////
void jump();
void release();
void keymove();
bool GetKeyStatus(int KeyNumber);
void UpdateKeyStatus();
void Judgment();
HRESULT InitDinput(HWND hInst);
HRESULT InitD3d(HWND hInst, const TCHAR* filePath);
HRESULT BuildDxDevice(HWND hInst, const TCHAR* filePath);
LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

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

// 背景
CUSTOMVERTEX v[4] =
{
{   0.0f,   0.0f, 0.0f, 1.0f, 0xffffffff,0.0f,0.0f},// 左上 
{ 700.0f,   0.0f, 0.0f, 1.0f, 0xffffffff,1.0f,0.0f},// 右上
{ 700.0f, 830.0f, 0.0f, 1.0f, 0xffffffff,1.0f,1.0f},// 右下
{   0.0f, 830.0f, 0.0f, 1.0f, 0xffffffff,0.0f,1.0f} // 左下
};

// プレイヤー
CUSTOMVERTEX v2[4] =
{
{  0.0f, 750.0f, 0.0f, 1.0f, 0xffffffff,0.0f,0.0f},// 左上 
{ 30.0f, 750.0f, 0.0f, 1.0f, 0xffffffff,1.0f,0.0f},// 右上
{ 30.0f, 780.0f, 0.0f, 1.0f, 0xffffffff,1.0f,1.0f},// 右下
{  0.0f, 780.0f, 0.0f, 1.0f, 0xffffffff,0.0f,1.0f} // 左下
};
// 画面下部分
CUSTOMVERTEX v3[4] =
{
{     0.0f, 530.0f, 0.0f, 1.0f, 0xffffffff,0.0f,0.0f},// 左上 
{   685.0f, 530.0f, 0.0f, 1.0f, 0xffffffff,1.0f,0.0f},// 右上
{   685.0f, 780.0f, 0.0f, 1.0f, 0xffffffff,1.0f,1.0f},// 右下
{     0.0f, 780.0f, 0.0f, 1.0f, 0xffffffff,0.0f,1.0f} // 左下
};
// 地面
CUSTOMVERTEX v4[4] =
{
{   0.0f, 780.0f, 0.0f, 1.0f, 0xffffffff,0.0f,0.0f},// 左上 
{ 700.0f, 780.0f, 0.0f, 1.0f, 0xffffffff,1.0f,0.0f},// 右上
{ 700.0f, 820.0f, 0.0f, 1.0f, 0xffffffff,1.0f,1.0f},// 右下
{   0.0f, 820.0f, 0.0f, 1.0f, 0xffffffff,0.0f,1.0f} // 左下
};
// 画面上部分
CUSTOMVERTEX v5[4] =
{
{   0.0f,  70.0f, 0.0f, 1.0f, 0xffffffff,0.0f,0.0f},// 左上 
{ 685.0f,  70.0f, 0.0f, 1.0f, 0xffffffff,1.0f,0.0f},// 右上
{ 685.0f, 531.0f, 0.0f, 1.0f, 0xffffffff,1.0f,1.0f},// 右下
{   0.0f, 531.0f, 0.0f, 1.0f, 0xffffffff,0.0f,1.0f} // 左下
};


INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szstr, INT iCmdshow)
{
	MSG msg;

	////////////////////////// ウィンドウ初期化 //////////////////////////////
	static char szAppName[] = "ORESAMA";
	WNDCLASSEX wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = wndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	//////////////////////////// ウィンドウ生成 //////////////////////////

	RegisterClassEx(&wndclass);
	HWND hInsT = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW, 0, 0, 700, 830, NULL, NULL, hInst, NULL);

	ShowWindow(hInsT, SW_SHOW);
	UpdateWindow(hInsT);

	/////////////////////////////// DirectInput ////////////////////////////////

	if (FAILED(InitDinput(hInsT))) {
		return 0;
	}

	/////////////////////////////// デバイス生成 /////////////////////////////////

	BuildDxDevice(hInsT, _T("hopping.jpg"));

	////////////////////////////// メインループ //////////////////////////

	timeBeginPeriod(1);
	//今の時間をtimeに保存。
	DWORD time = timeGetTime();
	DWORD prevtime = 0;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			time = timeGetTime();
			if (time - prevtime < 1000 / 60) {

				pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1.0, 0);
				pDevice->BeginScene();

				UpdateKeyStatus();
				if (GetKeyStatus(DIK_RETURN))
				{
					break;
				}
				jump();
				keymove();
				pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
				pDevice->SetTexture(0, pTexture);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, v, sizeof(CUSTOMVERTEX));
				pDevice->SetTexture(0, pTexture3);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, v3, sizeof(CUSTOMVERTEX));
				pDevice->SetTexture(0, pTexture4);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, v4, sizeof(CUSTOMVERTEX));
				pDevice->SetTexture(0, pTexture5);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, v5, sizeof(CUSTOMVERTEX));
				pDevice->SetTexture(0, pTexture2);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, v2, sizeof(CUSTOMVERTEX));
				pDevice->EndScene();
				pDevice->Present(0, 0, 0, 0);
			}


			prevtime = time;

		}
		timeEndPeriod(1);

	}
	if (pkey)
	{
		pkey->Unacquire();
	}

	release();
	return (int)msg.wParam;
}

HRESULT BuildDxDevice(HWND hInst, const TCHAR* filePath)
{
	//// InitD3d が初期化されているかどうか ////
	if (FAILED(InitD3d(hInst, filePath)))
	{
		return E_FAIL;
	}

	//// InitDinput 初期化されているかどうか ////
	if (FAILED(InitDinput(hInst)))
	{
		return E_FAIL;
	}

	//D3Dのポインタ変数にDirect3DCreate9関数(Direct3Dを作る関数)で作成したものを代入
	pDirect3d = Direct3DCreate9(D3D_SDK_VERSION);

	//// Direct3DCreate9 が ////
	if (pDirect3d == NULL)
	{
		return E_FAIL;
	}

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	//頂点情報をセット

	return S_OK;
}

HRESULT InitD3d(HWND hInst, const TCHAR* filePath)
{
	if (NULL == (pDirect3d = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		return E_FAIL;
	}

	//// バッファ設定 ////

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;

	//// デバイス生成 ////

	if (FAILED(pDirect3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hInst,
		D3DCREATE_MIXED_VERTEXPROCESSING, &d3dpp, &pDevice)))
	{
		return E_FAIL;
	}

	//// テクスチャフォーマット設定 ////

	if (FAILED(D3DXCreateTextureFromFile(pDevice, "back.png", &pTexture)))
	{
		return E_FAIL;
	}
	if (FAILED(D3DXCreateTextureFromFile(pDevice, "char_p3.png", &pTexture2)))
	{
		return E_FAIL;
	}
	if (FAILED(D3DXCreateTextureFromFile(pDevice, "game_row.png", &pTexture3)))
	{
		return E_FAIL;
	}
	if (FAILED(D3DXCreateTextureFromFile(pDevice, "ground.png", &pTexture4)))
	{
		return E_FAIL;
	}
	if (FAILED(D3DXCreateTextureFromFile(pDevice, "game_up.png", &pTexture5)))
	{
		return E_FAIL;
	}
	return S_OK;
}


////// directinput //////

HRESULT InitDinput(HWND hInst)
{
	HRESULT hr;

	//// DirectInput8の作成 ////
	if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID * *)& pDinput, NULL)))
	{
		return hr;
	}

	//// InputDeviceを作成 ////         * どの入力デバイスから情報を受け取るか
	if (FAILED(hr = pDinput->CreateDevice(GUID_SysKeyboard, &pkey, NULL)))
	{
		return hr;
	}

	//// デバイスのフォーマット設定,形式 ////　　　* キーボード c_dfDIKeyboard マウス c_dfDIMouse
	if (FAILED(hr = pkey->SetDataFormat(&c_dfDIKeyboard)))
	{
		return hr;
	}

	//// 協調レベルを設定 ////    * フォアグラウンド、バックグラウンド 排他的、非排他的 
	if (FAILED(hr = pkey->SetCooperativeLevel(hInst, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return hr;
	}

	//// 権限の取得 ////
	pkey->Acquire();

	return S_OK;
}

////// キーステータス更新関数 //////
void UpdateKeyStatus()
{
	HRESULT hr = pkey->Acquire();

	if ((hr == DI_OK) || (hr == S_FALSE))
	{
		pkey->GetDeviceState(sizeof(KeyState), &KeyState);
	}

}

////// キー入力関数 //////
bool GetKeyStatus(int KeyNumber)
{
	if (KeyState[KeyNumber] & 0x80)
	{
		return true; // 通ったらOK
	}
	return false; // 通らなかったら終了
}

////// 開放 //////

void release()
{
	pkey->Release();
	pkey = nullptr;
	pDinput->Release();
	pDinput = nullptr;
	pDevice->Release();
	pDevice = nullptr;
	pDirect3d->Release();
	pDirect3d = nullptr;
	pTexture->Release();
	pTexture = nullptr;
	pTexture2->Release();
	pTexture2 = nullptr;
	pTexture3->Release();
	pTexture3 = nullptr;
	pTexture4->Release();
	pTexture4 = nullptr;
}

////// ウィンドウプロシージャー //////

LRESULT CALLBACK wndProc(HWND hInst, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch ((CHAR)wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	}

	return DefWindowProc(hInst, iMsg, wParam, lParam);
}

////// 透過処理 //////

void initrender()
{
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
}

////// キーを押したときのテクスチャの移動 //////

void keymove()
{
	if (GetKeyStatus(DIK_A) | GetKeyStatus(DIK_LEFT))
	{
		v2[0].x -= 2.0f;
		v2[1].x -= 2.0f;
		v2[2].x -= 2.0f;
		v2[3].x -= 2.0f;

		v2[0].y -= 5.0f;
		v2[1].y -= 5.0f;
		v2[2].y -= 5.0f;
		v2[3].y -= 5.0f;

	}
	if (GetKeyStatus(DIK_D) | GetKeyStatus(DIK_RIGHT))
	{
		v2[0].x += 2.0f;
		v2[1].x += 2.0f;
		v2[2].x += 2.0f;
		v2[3].x += 2.0f;

		v2[0].y -= 5.0f;
		v2[1].y -= 5.0f;
		v2[2].y -= 5.0f;
		v2[3].y -= 5.0f;
	}

	Judgment();
}

//// ジャンプ判定 ////
void jump()
{
	GravityCount = GravityCount - 1.0f; // 常時かかる重力

	v2[0].y -= GravityCount;
	v2[1].y -= GravityCount;
	v2[2].y -= GravityCount;
	v2[3].y -= GravityCount;


	static bool JumpFlag = false; // ジャンプフラグ

	if (GetKeyStatus(DIK_SPACE) && !JumpFlag)
	{
		JumpFlag = true;
		SpeedCount += 25; // 初期速度
	}

	if (JumpFlag == true)
	{
		SpeedCount = SpeedCount - 1.0f; // ジャンプ時のみかかる重力

		v2[0].y -= SpeedCount;
		v2[1].y -= SpeedCount;
		v2[2].y -= SpeedCount;
		v2[3].y -= SpeedCount;

		if (v2[2].y > 780 && v2[3].y > 780)
		{
			GravityCount = 0;
			SpeedCount = 0;
			JumpFlag = false;

			v2[0].y = 750;
			v2[1].y = 750;
			v2[2].y = 780;
			v2[3].y = 780;
		}

	}

}

//// 当たり判定 ////

void Judgment()
{
	// 上のあたり判定
	if (v2[0].y < 0 && v2[1].y < 0)
	{
		v2[0].y = 0.0f;
		v2[1].y = 0.0f;
		v2[2].y = 30.0f;
		v2[3].y = 30.0f;
	}
	// 右のあたり判定
	if (v2[1].x > 685 && v2[2].x > 685)
	{
		v2[0].x = 655.0f;
		v2[1].x = 685.0f;
		v2[2].x = 685.0f;
		v2[3].x = 655.0f;
	}
	// 左のあたり判定
	if (v2[0].x < 0 && v2[3].x < 0)
	{
		v2[0].x = 0.0f;
		v2[1].x = 30.0f;
		v2[2].x = 30.0f;
		v2[3].x = 0.0f;
	}
	// 下のあたり判定
	if (v2[2].y > 780 && v2[3].y > 780)
	{
		GravityCount = 0;
		v2[0].y = 750;
		v2[1].y = 750;
		v2[2].y = 780;
		v2[3].y = 780;
	}

}

