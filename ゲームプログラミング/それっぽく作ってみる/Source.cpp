#include <windows.h>
#include<d3dx9.h>
#include<tchar.h>
#include<dinput.h>

// �E�B���h�E�̑傫�� //
#define WND_WIDTH 700 // �� X
#define WND_HEIGHT 830�@// �c�@Y

/////// �|�C���^��` ////// 

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

////// �v���g�^�C�v�錾 //////
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

// �w�i
CUSTOMVERTEX v[4] =
{
{   0.0f,   0.0f, 0.0f, 1.0f, 0xffffffff,0.0f,0.0f},// ���� 
{ 700.0f,   0.0f, 0.0f, 1.0f, 0xffffffff,1.0f,0.0f},// �E��
{ 700.0f, 830.0f, 0.0f, 1.0f, 0xffffffff,1.0f,1.0f},// �E��
{   0.0f, 830.0f, 0.0f, 1.0f, 0xffffffff,0.0f,1.0f} // ����
};

// �v���C���[
CUSTOMVERTEX v2[4] =
{
{  0.0f, 750.0f, 0.0f, 1.0f, 0xffffffff,0.0f,0.0f},// ���� 
{ 30.0f, 750.0f, 0.0f, 1.0f, 0xffffffff,1.0f,0.0f},// �E��
{ 30.0f, 780.0f, 0.0f, 1.0f, 0xffffffff,1.0f,1.0f},// �E��
{  0.0f, 780.0f, 0.0f, 1.0f, 0xffffffff,0.0f,1.0f} // ����
};
// ��ʉ�����
CUSTOMVERTEX v3[4] =
{
{     0.0f, 530.0f, 0.0f, 1.0f, 0xffffffff,0.0f,0.0f},// ���� 
{   685.0f, 530.0f, 0.0f, 1.0f, 0xffffffff,1.0f,0.0f},// �E��
{   685.0f, 780.0f, 0.0f, 1.0f, 0xffffffff,1.0f,1.0f},// �E��
{     0.0f, 780.0f, 0.0f, 1.0f, 0xffffffff,0.0f,1.0f} // ����
};
// �n��
CUSTOMVERTEX v4[4] =
{
{   0.0f, 780.0f, 0.0f, 1.0f, 0xffffffff,0.0f,0.0f},// ���� 
{ 700.0f, 780.0f, 0.0f, 1.0f, 0xffffffff,1.0f,0.0f},// �E��
{ 700.0f, 820.0f, 0.0f, 1.0f, 0xffffffff,1.0f,1.0f},// �E��
{   0.0f, 820.0f, 0.0f, 1.0f, 0xffffffff,0.0f,1.0f} // ����
};
// ��ʏ㕔��
CUSTOMVERTEX v5[4] =
{
{   0.0f,  70.0f, 0.0f, 1.0f, 0xffffffff,0.0f,0.0f},// ���� 
{ 685.0f,  70.0f, 0.0f, 1.0f, 0xffffffff,1.0f,0.0f},// �E��
{ 685.0f, 531.0f, 0.0f, 1.0f, 0xffffffff,1.0f,1.0f},// �E��
{   0.0f, 531.0f, 0.0f, 1.0f, 0xffffffff,0.0f,1.0f} // ����
};


INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szstr, INT iCmdshow)
{
	MSG msg;

	////////////////////////// �E�B���h�E������ //////////////////////////////
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

	//////////////////////////// �E�B���h�E���� //////////////////////////

	RegisterClassEx(&wndclass);
	HWND hInsT = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW, 0, 0, 700, 830, NULL, NULL, hInst, NULL);

	ShowWindow(hInsT, SW_SHOW);
	UpdateWindow(hInsT);

	/////////////////////////////// DirectInput ////////////////////////////////

	if (FAILED(InitDinput(hInsT))) {
		return 0;
	}

	/////////////////////////////// �f�o�C�X���� /////////////////////////////////

	BuildDxDevice(hInsT, _T("hopping.jpg"));

	////////////////////////////// ���C�����[�v //////////////////////////

	timeBeginPeriod(1);
	//���̎��Ԃ�time�ɕۑ��B
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
	//// InitD3d ������������Ă��邩�ǂ��� ////
	if (FAILED(InitD3d(hInst, filePath)))
	{
		return E_FAIL;
	}

	//// InitDinput ����������Ă��邩�ǂ��� ////
	if (FAILED(InitDinput(hInst)))
	{
		return E_FAIL;
	}

	//D3D�̃|�C���^�ϐ���Direct3DCreate9�֐�(Direct3D�����֐�)�ō쐬�������̂���
	pDirect3d = Direct3DCreate9(D3D_SDK_VERSION);

	//// Direct3DCreate9 �� ////
	if (pDirect3d == NULL)
	{
		return E_FAIL;
	}

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	//���_�����Z�b�g

	return S_OK;
}

HRESULT InitD3d(HWND hInst, const TCHAR* filePath)
{
	if (NULL == (pDirect3d = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		return E_FAIL;
	}

	//// �o�b�t�@�ݒ� ////

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;

	//// �f�o�C�X���� ////

	if (FAILED(pDirect3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hInst,
		D3DCREATE_MIXED_VERTEXPROCESSING, &d3dpp, &pDevice)))
	{
		return E_FAIL;
	}

	//// �e�N�X�`���t�H�[�}�b�g�ݒ� ////

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

	//// DirectInput8�̍쐬 ////
	if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID * *)& pDinput, NULL)))
	{
		return hr;
	}

	//// InputDevice���쐬 ////         * �ǂ̓��̓f�o�C�X��������󂯎�邩
	if (FAILED(hr = pDinput->CreateDevice(GUID_SysKeyboard, &pkey, NULL)))
	{
		return hr;
	}

	//// �f�o�C�X�̃t�H�[�}�b�g�ݒ�,�`�� ////�@�@�@* �L�[�{�[�h c_dfDIKeyboard �}�E�X c_dfDIMouse
	if (FAILED(hr = pkey->SetDataFormat(&c_dfDIKeyboard)))
	{
		return hr;
	}

	//// �������x����ݒ� ////    * �t�H�A�O���E���h�A�o�b�N�O���E���h �r���I�A��r���I 
	if (FAILED(hr = pkey->SetCooperativeLevel(hInst, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return hr;
	}

	//// �����̎擾 ////
	pkey->Acquire();

	return S_OK;
}

////// �L�[�X�e�[�^�X�X�V�֐� //////
void UpdateKeyStatus()
{
	HRESULT hr = pkey->Acquire();

	if ((hr == DI_OK) || (hr == S_FALSE))
	{
		pkey->GetDeviceState(sizeof(KeyState), &KeyState);
	}

}

////// �L�[���͊֐� //////
bool GetKeyStatus(int KeyNumber)
{
	if (KeyState[KeyNumber] & 0x80)
	{
		return true; // �ʂ�����OK
	}
	return false; // �ʂ�Ȃ�������I��
}

////// �J�� //////

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

////// �E�B���h�E�v���V�[�W���[ //////

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

////// ���ߏ��� //////

void initrender()
{
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
}

////// �L�[���������Ƃ��̃e�N�X�`���̈ړ� //////

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

//// �W�����v���� ////
void jump()
{
	GravityCount = GravityCount - 1.0f; // �펞������d��

	v2[0].y -= GravityCount;
	v2[1].y -= GravityCount;
	v2[2].y -= GravityCount;
	v2[3].y -= GravityCount;


	static bool JumpFlag = false; // �W�����v�t���O

	if (GetKeyStatus(DIK_SPACE) && !JumpFlag)
	{
		JumpFlag = true;
		SpeedCount += 25; // �������x
	}

	if (JumpFlag == true)
	{
		SpeedCount = SpeedCount - 1.0f; // �W�����v���݂̂�����d��

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

//// �����蔻�� ////

void Judgment()
{
	// ��̂����蔻��
	if (v2[0].y < 0 && v2[1].y < 0)
	{
		v2[0].y = 0.0f;
		v2[1].y = 0.0f;
		v2[2].y = 30.0f;
		v2[3].y = 30.0f;
	}
	// �E�̂����蔻��
	if (v2[1].x > 685 && v2[2].x > 685)
	{
		v2[0].x = 655.0f;
		v2[1].x = 685.0f;
		v2[2].x = 685.0f;
		v2[3].x = 655.0f;
	}
	// ���̂����蔻��
	if (v2[0].x < 0 && v2[3].x < 0)
	{
		v2[0].x = 0.0f;
		v2[1].x = 30.0f;
		v2[2].x = 30.0f;
		v2[3].x = 0.0f;
	}
	// ���̂����蔻��
	if (v2[2].y > 780 && v2[3].y > 780)
	{
		GravityCount = 0;
		v2[0].y = 750;
		v2[1].y = 750;
		v2[2].y = 780;
		v2[3].y = 780;
	}

}

