#include <windows.h>
#include<d3dx9.h>
#include<tchar.h>
#include<dinput.h>

#include"struct.h"

/////// ポインタ定義 ////// 

LPDIRECT3DTEXTURE9 pTexture; 
LPDIRECT3DTEXTURE9 pTexture2;
LPDIRECT3D9 pDirect3d;
LPDIRECT3DDEVICE9 pDevice;
LPDIRECTINPUT8 pDinput = NULL;
LPDIRECTINPUTDEVICE8 pkey = NULL;

////// プロトタイプ宣言 //////

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
	HWND hInsT = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW, 0, 0, 640, 480, NULL, NULL, hInst, NULL);

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
				keymove();
				pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
				pDevice->SetTexture(0, pTexture);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, v, sizeof(CUSTOMVERTEX));
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

	if (FAILED(D3DXCreateTextureFromFile(pDevice, "brownblock.png", &pTexture)))
	{
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFile(pDevice, "brownblock.png", &pTexture2)))
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
	GravityCount = GravityCount - 2.0f;

	static bool JumpFlag = false;

	if (GetKeyStatus(DIK_SPACE) && !JumpFlag)
	{
		JumpFlag = true;
		SpeedCount += 30;
	}

	if (JumpFlag == true)
	{
		SpeedCount = SpeedCount - 3.1f;

		v[0].y -= SpeedCount;
		v[1].y -= SpeedCount;
		v[2].y -= SpeedCount;
		v[3].y -= SpeedCount;

		if ((v[2].y > 341 && v[3].y > 341) && ( 250 < v[2].x  < 350 )&&(v[3].x < 250 ))
		{
			SpeedCount = 0;
			JumpFlag = false;

			v[0].y = 241;
			v[1].y = 241;
			v[2].y = 341;
			v[3].y = 341;
		}
		else if (v[2].y > 441 && v[3].y > 441)
		{
			SpeedCount = 0;
			JumpFlag = false;

			v[0].y = 341;
			v[1].y = 341;
			v[2].y = 441;
			v[3].y = 441;
		}

	}

	// 「 W 」キーの動き
	if (GetKeyStatus(DIK_W) | GetKeyStatus(DIK_UP))
	{
		v[0].y -= 5.0f;
		v[1].y -= 5.0f;
		v[2].y -= 5.0f;
		v[3].y -= 5.0f;

	}
	if (GetKeyStatus(DIK_S) | GetKeyStatus(DIK_DOWN))
	{
		v[0].y += 5.0f;
		v[1].y += 5.0f;
		v[2].y += 5.0f;
		v[3].y += 5.0f;

	}
	if (GetKeyStatus(DIK_A) | GetKeyStatus(DIK_LEFT))
	{
		v[0].x -= 5.0f;
		v[1].x -= 5.0f;
		v[2].x -= 5.0f;
		v[3].x -= 5.0f;

	}
	if (GetKeyStatus(DIK_D) | GetKeyStatus(DIK_RIGHT))
	{
		v[0].x += 5.0f;
		v[1].x += 5.0f;
		v[2].x += 5.0f;
		v[3].x += 5.0f;

	}

	Judgment();
}

void Judgment()
{
	// 上のあたり判定
	if (v[0].y < 0 && v[1].y < 0)
	{
		v[0].y += 5.0f;
		v[1].y += 5.0f;
		v[2].y += 5.0f;
		v[3].y += 5.0f;
	}
	// 右のあたり判定
	if (v[1].x > 640 && v[2].x > 640)
	{
		v[0].x -= 5.0f;
		v[1].x -= 5.0f;
		v[2].x -= 5.0f;
		v[3].x -= 5.0f;
	}
	// 左のあたり判定
	if (v[0].x < 0 && v[3].x < 0)
	{
		v[0].x += 5.0f;
		v[1].x += 5.0f;
		v[2].x += 5.0f;
		v[3].x += 5.0f;
	}
	// 下のあたり判定
	if ((v[2].y > 341 && v[3].y > 341) && (250 < v[2].x < 350) && (v[3].x < 250))
	{
		v[0].y = 241;
		v[1].y = 241;
		v[2].y = 341;
		v[3].y = 341;
	} else 
		if (v[2].y > 441 && v[3].y > 441)
	{
		v[0].y -= 5.0f;
		v[1].y -= 5.0f;
		v[2].y -= 5.0f;
		v[3].y -= 5.0f;
	}
	// ブロックのあたり判定
		// 左のあたり判定
	if ((v[0].x < 250.2 && v[3].x < 250.2) && v[3].y > 341)
	{
		v[0].x += 5.0f;
		v[1].x += 5.0f;
		v[2].x += 5.0f;
		v[3].x += 5.0f;
	}

}
