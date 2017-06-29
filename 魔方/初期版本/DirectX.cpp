#include "DirectX.h"

LPDIRECT3D9 d3d;			//D3D9�ӿ�
LPDIRECT3DDEVICE9 d3ddev;	//D3D9�豸

LPDIRECTINPUT8 dinput;		//����ӿ�
LPDIRECTINPUTDEVICE8 dikeyboard;	//���̽ӿ�
char keys[256];

LPDIRECT3DTEXTURE9 red, yellow, blue, green, white, orange, black;	//ħ������ɫ

LPDIRECT3DVERTEXBUFFER9 vb;	//D3D���㻺��
LPDIRECT3DINDEXBUFFER9 ib;	//D3D���㻺��

DWORD current_time = timeGetTime();		//��ʱ��
DWORD fps = 0;							
DWORD delay = 16;							//һ֡���
DWORD frame_timer = timeGetTime();			//֡��ʱ��
DWORD key_down_counter;						//�������������30֡һ��

//�������ʼλ�ã�����Ŀ�꣬��Ӱ�����ӽ�
D3DXVECTOR3 pos(6.0f, 6.0f, -6.0f);
D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 up(-1.0f, 8.0f, 1.0f);

//�������
D3DMATERIAL9 material;

//�ƹ�
D3DLIGHT9 light;

//��������
LPDIRECT3DTEXTURE9 LoadTexture(const char * filename)
{
	LPDIRECT3DTEXTURE9 texture = nullptr;
	D3DXCreateTextureFromFile(d3ddev, filename, &texture);
	if (!texture)
		return nullptr;
	return texture;
}



// ����任
void WorldTransform()
{
	
	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 0.0f);
	d3ddev->SetTransform(D3DTS_WORLD, &matWorld);
}

// ȡ���任��eyeΪ��Ӱ��λ�ã�lookAtΪ��Ӱ������upΪ��Ӱ���Ϸ���
void ViewTransform(D3DXVECTOR3 eye, D3DXVECTOR3 lookAt, D3DXVECTOR3 up)
{
	D3DXMATRIX View;
	D3DXMatrixLookAtLH(&View, &eye, &lookAt, &up);
	d3ddev->SetTransform(D3DTS_VIEW, &View);
}

// ͶӰ�任��fieldOfViewΪ��ֱ��Ұ�Ƕȣ�aspectRatioΪ��߱ȣ�nearRangeΪǰ�ü�����룬farRangeΪ��ü������
void PerspectiveTransform(float fieldOfView, float aspectRatio, float nearRange, float farRange)
{
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, fieldOfView, aspectRatio, nearRange, farRange);
	d3ddev->SetTransform(D3DTS_PROJECTION, &proj);
}

//Direct3D�ӿ����豸��ʼ��
bool Direct3D_Init(HWND window)
{
	//����D3D9����
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d)
		return false;
	//��ʼ�������豸
	if (!DirectInput_Init(window))
		return false;


	//���D3D����
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = SCREENW;		//�󱸻��������
	d3dpp.BackBufferHeight = SCREENH;		//�󱸻������߶�
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;				//�󱸻�������Ŀ
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;		//���ڻ�
	d3dpp.hDeviceWindow = window;
	d3dpp.EnableAutoDepthStencil = 1;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	
	//�����豸
	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev
	);

	if (!d3ddev)
		return false;

	//�����������������ʼ��
	d3ddev->CreateVertexBuffer(
		648 * sizeof(Vetrex),
		D3DUSAGE_WRITEONLY,
		D3DFVF_MYVETREX,
		D3DPOOL_MANAGED,
		&vb,
		0
	);

	d3ddev->CreateIndexBuffer(
		972 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&ib,
		0
	);

	//Direct������󴴽�
	DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&dinput,
		NULL);

	//��ʼ������
	dinput->CreateDevice(GUID_SysKeyboard, &dikeyboard, NULL);
	dikeyboard->SetDataFormat(&c_dfDIKeyboard);
	dikeyboard->SetCooperativeLevel(window, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	dikeyboard->Acquire();

	//���ù�����
	d3ddev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	d3ddev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	d3ddev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);


	//�����ȡ
	red = LoadTexture("Resource/Red.png");
	green = LoadTexture("Resource/Green.png");
	yellow = LoadTexture("Resource/Yellow.png");
	blue = LoadTexture("Resource/Blue.png");
	white = LoadTexture("Resource/White.png");
	orange = LoadTexture("Resource/Orange.png");
	black = LoadTexture("Resource/Black.png");

	//�������
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Emissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Power = 5.0f;

	d3ddev->SetMaterial(&material);

	//�ƹ��ʼ��
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Direction = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	d3ddev->SetLight(0, &light);
	d3ddev->LightEnable(0, true);

	return true;
}





void Direct3D_Shutdown()
{
	if (d3ddev) d3ddev->Release();
	if (d3d) d3d->Release();
	if (vb) vb->Release();

	if (red) red->Release();
	if (green) green->Release();
	if (blue) blue->Release();
	if (white) white->Release();
	if (yellow) yellow->Release();
	if (orange) orange->Release();
	if (black) black->Release();

	DirectInput_Shutdown();
}

//DirectInput��ʼ��
bool DirectInput_Init(HWND hwnd)
{
	//��ʼ��DirectInput����
	DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&dinput,
		NULL);

	//��ʼ������
	dinput->CreateDevice(GUID_SysKeyboard, &dikeyboard, NULL);
	dikeyboard->SetDataFormat(&c_dfDIKeyboard);
	dikeyboard->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	dikeyboard->Acquire();
	return true;
}

//DirectInput����
void DirectInput_Update()
{
	//���¼���
	dikeyboard->Poll();
	if (!SUCCEEDED(dikeyboard->GetDeviceState(256, (LPVOID)&keys)))
	{
		//�����豸��ʧ���������»�ȡ
		dikeyboard->Acquire();
	}
}

//DirectInput�ͷ�
void DirectInput_Shutdown()
{
	if (dikeyboard)
	{
		dikeyboard->Unacquire();
		dikeyboard->Release();
		dikeyboard = NULL;
	}
}

bool Key_Down(int key)
{
	return (bool)(keys[key] & 0x80);
}

//����sin(D3DX_PI)�����Ϊ0�����
double MySin(double _X)
{
	double res = sin(_X);
	return fabs(res) < 10e-5 ? 0 : res;
}

//����cos(D3DX_PI / 2)�����Ϊ0�����
double MyCos(double _X)
{
	double res = cos(_X);
	return fabs(res) < 10e-5 ? 0 : res;
}


