#include "DirectX.h"
#include "Rubik.h"
extern LPDIRECT3D9 d3d;
extern LPDIRECT3DDEVICE9 d3ddev;

extern LPDIRECT3DVERTEXBUFFER9 vb;		
extern LPDIRECT3DINDEXBUFFER9 ib;

extern D3DXVECTOR3 pos;
extern D3DXVECTOR3 target;
extern D3DXVECTOR3 up;

extern LPDIRECT3DTEXTURE9 red, yellow, blue, green, white, orange, black;

extern Rubik rubik;

extern DWORD current_time;
extern DWORD fps;
extern DWORD delay;					//һ֡���
extern DWORD frame_timer;			//֡��ʱ��
extern DWORD key_down_counter;		//�������������30֡һ��

DWORD start_time;					//��Ϸ��ʼ��ʱ��

//��ʼ��
bool Game_Init(HWND window)
{
	Direct3D_Init(window);

	rubik.reset();

	

	//����任
	WorldTransform();

	//�ӽǱ任
	ViewTransform(pos, target, up);

	//ͶӰ�任
	PerspectiveTransform(D3DX_PI * 0.4f, (float)SCREENW / (float)SCREENH, 1.0f, 1000.0f);


	d3ddev->SetRenderState(D3DRS_LIGHTING, true);
	d3ddev->SetRenderState(D3DRS_NORMALIZENORMALS, true);


	return true;
}

void Game_Run(HWND window)
{
	DirectInput_Update();
	

	if (timeGetTime() - current_time > delay)
	{
		rubik.update();
		Key_Update();
		//����
		d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(117, 108, 131), 1.0f, 0);

		if (d3ddev->BeginScene())									//��ʼ��Ⱦ
		{
			rubik.show();

			d3ddev->EndScene();										//ֹͣ��Ⱦ
			d3ddev->Present(nullptr, nullptr, nullptr, nullptr);	//ǰ̨��̨����
		}

		fps++;
		
		current_time = timeGetTime();
	}

	//����֡����60����
	if (timeGetTime() - frame_timer > 1000)
	{
		if (fps > 66)
			delay++;
		else if (fps < 54)
			delay--;
		frame_timer = timeGetTime();
		fps = 0;
	}
	//���ħ����ԭ
	if (!rubik.shuffle_times)
		rubik.is_complete(window);

	if (KEY_DOWN(VK_ESCAPE))
		PostMessage(window, WM_DESTROY, 0, 0);

}

void Game_End()
{
	Direct3D_Shutdown();

	red->Release();
	blue->Release();
	green->Release();
	yellow->Release();
	white->Release();
	orange->Release();
	black->Release();
}

void Key_Update()
{
	if (key_down_counter)
	{
		key_down_counter--;
		return;
	}
	if (rubik.shuffle_times)
		return;

	//š��ħ������
	//˫��š������
	if (Key_Down(DIK_LCONTROL) && Key_Down(DIK_I))
	{
		rubik.rotate_YZ(2, false);
		rubik.rotate_YZ(1, false);
	}
	else if (Key_Down(DIK_LCONTROL) && Key_Down(DIK_K))
	{
		rubik.rotate_YZ(2, true);
		rubik.rotate_YZ(1, true);
	}
	else if (Key_Down(DIK_LCONTROL) && Key_Down(DIK_J))
	{
		rubik.rotate_XZ(2, false);
		rubik.rotate_XZ(1, false);
	}
	else if (Key_Down(DIK_LCONTROL) && Key_Down(DIK_L))
	{
		rubik.rotate_XZ(2, true);
		rubik.rotate_XZ(1, true);
	}
	else if (Key_Down(DIK_LCONTROL) && Key_Down(DIK_U))
	{
		rubik.rotate_XY(0, true);
		rubik.rotate_XY(1, true);
	}
	else if (Key_Down(DIK_LCONTROL) && Key_Down(DIK_O))
	{
		rubik.rotate_XY(0, false);
		rubik.rotate_XY(1, false);
	}
	//����š������
	else if (Key_Down(DIK_I))
		rubik.rotate_YZ(2, false);
	else if (Key_Down(DIK_K))
		rubik.rotate_YZ(2, true);
	else if (Key_Down(DIK_J))
		rubik.rotate_XZ(2, false);
	else if (Key_Down(DIK_L))
		rubik.rotate_XZ(2, true);
	else if (Key_Down(DIK_U))
		rubik.rotate_XY(0, true);
	else if (Key_Down(DIK_O))
		rubik.rotate_XY(0, false);

	else if (Key_Down(DIK_T))
		rubik.rotate_YZ(1, false);
	else if (Key_Down(DIK_G))
		rubik.rotate_YZ(1, true);
	else if (Key_Down(DIK_F))
		rubik.rotate_XZ(1, false);
	else if (Key_Down(DIK_H))
		rubik.rotate_XZ(1, true);
	else if (Key_Down(DIK_R))
		rubik.rotate_XY(1, true);
	else if (Key_Down(DIK_Y))
		rubik.rotate_XY(1, false);

	else if (Key_Down(DIK_W))
		rubik.rotate_YZ(0, false);
	else if (Key_Down(DIK_S))
		rubik.rotate_YZ(0, true);
	else if (Key_Down(DIK_A))
		rubik.rotate_XZ(0, false);
	else if (Key_Down(DIK_D))
		rubik.rotate_XZ(0, true);
	else if (Key_Down(DIK_Q))
		rubik.rotate_XY(2, true);
	else if (Key_Down(DIK_E))
		rubik.rotate_XY(2, false);

	//��תħ������
	else if (Key_Down(DIK_UP))
		rubik.view_rotate_X(false);
	else if (Key_Down(DIK_DOWN))
		rubik.view_rotate_X(true);
	else if (Key_Down(DIK_LEFT))
		rubik.view_rotate_Y(false);
	else if (Key_Down(DIK_RIGHT))
		rubik.view_rotate_Y(true);
	else if (Key_Down(DIK_PGUP))
		rubik.view_rotate_Z(true);
	else if (Key_Down(DIK_PGDN))
		rubik.view_rotate_Z(false);
	//������Ϸ
	else if (Key_Down(DIK_F11))
		rubik.reset();
	else
		return;

	if (!start_time) start_time = timeGetTime();
	key_down_counter = 18;
}

//�����ȸ�����ת��

std::string to_string(double _Val, int precision)
{
	std::string str;
	str += "%." + std::to_string(precision) + "f";
	return (std::_Floating_to_string(str.c_str(), _Val));
}