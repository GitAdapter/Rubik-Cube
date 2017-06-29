#include "Rubik.h"

#include <algorithm>
extern LPDIRECT3DVERTEXBUFFER9 vb;
extern LPDIRECT3DINDEXBUFFER9 ib;
extern LPDIRECT3DDEVICE9 d3ddev;
extern LPDIRECT3DTEXTURE9 red, yellow, blue, green, white, orange, black;

extern DWORD key_down_counter;
extern DWORD start_time;
//ħ��
Rubik rubik;


//����ħ��
void Rubik::reset()
{
	//��ʼ����������ꡢ���������������꣬������������ɫ��ȫͿ��
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				//����
				cube[i][j][k].v[0] = Vetrex(-3.0f + 2.0f * i, -1.0f + 2.0f * j, -3.0f + 2.0f * k, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
				cube[i][j][k].v[1] = Vetrex(-3.0f + 2.0f * i, -1.0f + 2.0f * j, -1.0f + 2.0f * k, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
				cube[i][j][k].v[2] = Vetrex(-1.0f + 2.0f * i, -1.0f + 2.0f * j, -1.0f + 2.0f * k, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
				cube[i][j][k].v[3] = Vetrex(-1.0f + 2.0f * i, -1.0f + 2.0f * j, -3.0f + 2.0f * k, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
				cube[i][j][k].top = black;

				//����
				cube[i][j][k].v[4] = Vetrex(-1.0f + 2.0f * i, -3.0f + 2.0f * j, -3.0f + 2.0f * k, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
				cube[i][j][k].v[5] = Vetrex(-1.0f + 2.0f * i, -3.0f + 2.0f * j, -1.0f + 2.0f * k, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
				cube[i][j][k].v[6] = Vetrex(-3.0f + 2.0f * i, -3.0f + 2.0f * j, -1.0f + 2.0f * k, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
				cube[i][j][k].v[7] = Vetrex(-3.0f + 2.0f * i, -3.0f + 2.0f * j, -3.0f + 2.0f * k, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
				cube[i][j][k].bottom = black;

				//����
				cube[i][j][k].v[8] = Vetrex(-3.0f + 2.0f * i, -3.0f + 2.0f * j, -1.0f + 2.0f * k, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
				cube[i][j][k].v[9] = Vetrex(-3.0f + 2.0f * i, -1.0f + 2.0f * j, -1.0f + 2.0f * k, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
				cube[i][j][k].v[10] = Vetrex(-3.0f + 2.0f * i, -1.0f + 2.0f * j, -3.0f + 2.0f * k, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
				cube[i][j][k].v[11] = Vetrex(-3.0f + 2.0f * i, -3.0f + 2.0f * j, -3.0f + 2.0f * k, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
				cube[i][j][k].left = black;

				//����
				cube[i][j][k].v[12] = Vetrex(-1.0f + 2.0f * i, -3.0f + 2.0f * j, -3.0f + 2.0f * k, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
				cube[i][j][k].v[13] = Vetrex(-1.0f + 2.0f * i, -1.0f + 2.0f * j, -3.0f + 2.0f * k, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
				cube[i][j][k].v[14] = Vetrex(-1.0f + 2.0f * i, -1.0f + 2.0f * j, -1.0f + 2.0f * k, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
				cube[i][j][k].v[15] = Vetrex(-1.0f + 2.0f * i, -3.0f + 2.0f * j, -1.0f + 2.0f * k, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
				cube[i][j][k].right = black;

				//����
				cube[i][j][k].v[16] = Vetrex(-3.0f + 2.0f * i, -3.0f + 2.0f * j, -3.0f + 2.0f * k, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
				cube[i][j][k].v[17] = Vetrex(-3.0f + 2.0f * i, -1.0f + 2.0f * j, -3.0f + 2.0f * k, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
				cube[i][j][k].v[18] = Vetrex(-1.0f + 2.0f * i, -1.0f + 2.0f * j, -3.0f + 2.0f * k, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
				cube[i][j][k].v[19] = Vetrex(-1.0f + 2.0f * i, -3.0f + 2.0f * j, -3.0f + 2.0f * k, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
				cube[i][j][k].front = black;

				//����
				cube[i][j][k].v[20] = Vetrex(-1.0f + 2.0f * i, -3.0f + 2.0f * j, -1.0f + 2.0f * k, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
				cube[i][j][k].v[21] = Vetrex(-1.0f + 2.0f * i, -1.0f + 2.0f * j, -1.0f + 2.0f * k, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
				cube[i][j][k].v[22] = Vetrex(-3.0f + 2.0f * i, -1.0f + 2.0f * j, -1.0f + 2.0f * k, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
				cube[i][j][k].v[23] = Vetrex(-3.0f + 2.0f * i, -3.0f + 2.0f * j, -1.0f + 2.0f * k, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
				cube[i][j][k].rear = black;

				//һ��ʼ��û��ת��״̬
				cube[i][j][k].rotating = false;
			}
		}
	}
	//��ʼ������ɫ
	for (int i = 0; i < 3; ++i)
		for (int k = 0; k < 3; ++k)
		{
			cube[i][2][k].top = green;
			cube[i][0][k].bottom = blue;
		}

	for (int j = 0; j < 3; ++j)
		for (int k = 0; k < 3; ++k)
		{
			cube[0][j][k].left = red;
			cube[2][j][k].right = orange;
		}

	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
		{
			cube[i][j][0].front = white;
			cube[i][j][2].rear = yellow;
		}

	shuffle_times = 30;
	degree = 0;
	start_time = 0;
	key_down_counter = 0;
	XY = YZ = XZ = false;
	complete = false;
	
}

//����ħ��
void Rubik::update()
{
	if (degree > 0) 
		degree -= 6;
	else if (degree < 0) 
		degree += 6;
	else if (!degree && (XY || XZ || YZ))
	{
		XY = XZ = YZ = false;
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				for (int k = 0; k < 3; ++k)
					cube[i][j][k].rotating = false;
	}

	//�����������
	if (!degree && shuffle_times && !key_down_counter)
	{
		int func = rand() % 12;
		int index = rand() % 3;
		bool inverse = rand() % 2;
		if (func < 3)
			rotate_YZ(index, inverse);
		else if (func < 6)
			rotate_XZ(index, inverse);
		else if (func < 9)
			rotate_XY(index, inverse);
		else if (func < 10)
			view_rotate_X(inverse);
		else if (func < 11)
			view_rotate_Y(inverse);
		else
			view_rotate_Z(inverse);
		shuffle_times--;
		key_down_counter = 18;
	}
}

//��ʾħ��
void Rubik::show()
{
	D3DXVECTOR4 vec4;
	D3DXVECTOR3 vec3;
	D3DXMATRIX matrix;

	//������ת������ȡ�ö�Ӧ����ת����
	if (rubik.XY)
		D3DXMatrixRotationZ(&matrix, D3DX_PI * rubik.degree / 180);
	else if (rubik.YZ)
		D3DXMatrixRotationX(&matrix, D3DX_PI * rubik.degree / 180);
	else if (rubik.XZ)
		D3DXMatrixRotationY(&matrix, D3DX_PI * rubik.degree / 180);

	//���㻺��������
	int pos = 0;
	Vetrex* vetrices;
	vb->Lock(0, 0, (void**)&vetrices, 0);
	
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			for (int k = 0; k < 3; ++k)
				for (int l = 0; l < 24; ++l)
				{
					//�÷�������ת�Ļ�����������ת����
					if (rubik.cube[i][j][k].rotating)
					{	
						D3DXVec3Transform(&vec4, &rubik.cube[i][j][k].v[l].pos, &matrix);
						vec3.x = vec4.x; vec3.y = vec4.y; vec3.z = vec4.z;
						vetrices[pos] = rubik.cube[i][j][k].v[l];
						vetrices[pos++].pos = vec3;
					}
					else
						vetrices[pos++] = rubik.cube[i][j][k].v[l];
				}
	vb->Unlock();

	pos = 0;
	WORD* indices;
	ib->Lock(0, 0, (void**)&indices, 0);

	for (int i = 0; i < 162; ++i)
	{
		indices[i * 6 + 0] = 4 * i + 0;
		indices[i * 6 + 1] = 4 * i + 1;
		indices[i * 6 + 2] = 4 * i + 2;
		indices[i * 6 + 3] = 4 * i + 2;
		indices[i * 6 + 4] = 4 * i + 3;
		indices[i * 6 + 5] = 4 * i + 0;
	}
	ib->Unlock();

	//��ʾ����
	d3ddev->SetStreamSource(0, vb, 0, sizeof(Vetrex));
	d3ddev->SetFVF(D3DFVF_MYVETREX);
	d3ddev->SetIndices(ib);
	
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			for (int k = 0; k < 3; ++k)
			{
				d3ddev->SetTexture(0, rubik.cube[i][j][k].top);
				d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, (i * 3 * 3 + j * 3 + k) * 24, 4, (i * 3 * 3 + j * 3 + k) * 36, 2);

				d3ddev->SetTexture(0, rubik.cube[i][j][k].bottom);
				d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, (i * 3 * 3 + j * 3 + k) * 24 + 4, 4, (i * 3 * 3 + j * 3 + k) * 36 + 6, 2);

				d3ddev->SetTexture(0, rubik.cube[i][j][k].left);
				d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, (i * 3 * 3 + j * 3 + k) * 24 + 8, 4, (i * 3 * 3 + j * 3 + k) * 36 + 12, 2);

				d3ddev->SetTexture(0, rubik.cube[i][j][k].right);
				d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, (i * 3 * 3 + j * 3 + k) * 24 + 12, 4, (i * 3 * 3 + j * 3 + k) * 36 + 18, 2);

				d3ddev->SetTexture(0, rubik.cube[i][j][k].front);
				d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, (i * 3 * 3 + j * 3 + k) * 24 + 16, 4, (i * 3 * 3 + j * 3 + k) * 36 + 24, 2);

				d3ddev->SetTexture(0, rubik.cube[i][j][k].rear);
				d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, (i * 3 * 3 + j * 3 + k) * 24 + 20, 4, (i * 3 * 3 + j * 3 + k) * 36 + 30, 2);
			}
}

//���ħ���Ƿ���ɣ������ɣ���������ʱ�Ĵ���
bool Rubik::is_complete(HWND window)
{
	if (complete)
		return true;
	if (degree)		//ħ����ֹͣת������ܼ��
		return false;

	LPDIRECT3DTEXTURE9 color1 = cube[0][2][0].top;
	LPDIRECT3DTEXTURE9 color2 = cube[0][0][0].bottom;
	//���������
	for (int i = 0; i < 3; ++i)
		for (int k = 0; k < 3; ++k)
			if (cube[i][2][k].top != color1 || cube[i][0][k].bottom != color2)
				return false;
	
	//����������
	color1 = cube[0][0][0].left;
	color2 = cube[2][0][0].right;
	for (int j = 0; j < 3; ++j)
		for (int k = 0; k < 3; ++k)
			if (cube[0][j][k].left != color1 || cube[2][j][k].right != color2)
				return false;

	//���汳����
	color1 = cube[0][0][0].front;
	color2 = cube[0][0][2].rear;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			if (cube[i][j][0].front != color1 || cube[i][j][2].rear != color2)
				return false;

	MessageBox(window, (std::string("��ʱ��") + to_string((double)(timeGetTime() - start_time) / 1e3, 3) + "s\n��Ϸ���水F11������").c_str(),
		"��ϲ���", MB_OK);

	complete = true;
	return true;
}

//YZ����ת��indexΪ0ʱת�����棬1ת���м䣬2ת������
void Rubik::rotate_YZ(int index, bool inverse)
{
	for (int i = inverse ? 7 : 0; inverse ? (i >= 2) : (i < 6); inverse ? --i : ++i)
	{
		std::swap(rubik.cube[index][(int)(1 + 1.5 * MySin(D3DX_PI / 4 * i))][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * i))].front,
			rubik.cube[index][(int)(1 + 1.5 * MySin(D3DX_PI / 4 * (2 + i)))][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * (2 + i)))].bottom);
		std::swap(rubik.cube[index][(int)(1 + 1.5 * MySin(D3DX_PI / 4 * i))][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * i))].top,
			rubik.cube[index][(int)(1 + 1.5 * MySin(D3DX_PI / 4 * (2 + i)))][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * (2 + i)))].front);
		std::swap(rubik.cube[index][(int)(1 + 1.5 * MySin(D3DX_PI / 4 * i))][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * i))].rear,
			rubik.cube[index][(int)(1 + 1.5 * MySin(D3DX_PI / 4 * (2 + i)))][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * (2 + i)))].top);
		std::swap(rubik.cube[index][(int)(1 + 1.5 * MySin(D3DX_PI / 4 * i))][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * i))].bottom,
			rubik.cube[index][(int)(1 + 1.5 * MySin(D3DX_PI / 4 * (2 + i)))][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * (2 + i)))].rear);

		std::swap(rubik.cube[index][(int)(1 + 1.5 * MySin(D3DX_PI / 4 * i))][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * i))].left,
			rubik.cube[index][(int)(1 + 1.5 * MySin(D3DX_PI / 4 * (2 + i)))][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * (2 + i)))].left);
		std::swap(rubik.cube[index][(int)(1 + 1.5 * MySin(D3DX_PI / 4 * i))][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * i))].right,
			rubik.cube[index][(int)(1 + 1.5 * MySin(D3DX_PI / 4 * (2 + i)))][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * (2 + i)))].right);
	}
	
	rubik.degree = inverse ? 90 : -90;
	rubik.YZ = true;
	for (int j = 0; j < 3; ++j)
		for (int k = 0; k < 3; ++k)
			rubik.cube[index][j][k].rotating = true;

}

//XZ����ת��indexΪ0ʱת�����棬1ת���м䣬2ת������
void Rubik::rotate_XZ(int index, bool inverse)
{
	for (int i = inverse ? 0 : 7; inverse ? (i < 6) : (i >= 2); inverse ? ++i : --i)
	{
		std::swap(rubik.cube[(int)(1 + 1.5 * MySin(D3DX_PI / 4 * i))][index][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * i))].front,
			rubik.cube[(int)(1 + 1.5 * MySin(D3DX_PI / 4 * (2 + i)))][index][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * (2 + i)))].left);
		std::swap(rubik.cube[(int)(1 + 1.5 * MySin(D3DX_PI / 4 * i))][index][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * i))].right,
			rubik.cube[(int)(1 + 1.5 * MySin(D3DX_PI / 4 * (2 + i)))][index][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * (2 + i)))].front);
		std::swap(rubik.cube[(int)(1 + 1.5 * MySin(D3DX_PI / 4 * i))][index][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * i))].rear,
			rubik.cube[(int)(1 + 1.5 * MySin(D3DX_PI / 4 * (2 + i)))][index][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * (2 + i)))].right);
		std::swap(rubik.cube[(int)(1 + 1.5 * MySin(D3DX_PI / 4 * i))][index][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * i))].left,
			rubik.cube[(int)(1 + 1.5 * MySin(D3DX_PI / 4 * (2 + i)))][index][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * (2 + i)))].rear);

		std::swap(rubik.cube[(int)(1 + 1.5 * MySin(D3DX_PI / 4 * i))][index][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * i))].top,
			rubik.cube[(int)(1 + 1.5 * MySin(D3DX_PI / 4 * (2 + i)))][index][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * (2 + i)))].top);
		std::swap(rubik.cube[(int)(1 + 1.5 * MySin(D3DX_PI / 4 * i))][index][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * i))].bottom,
			rubik.cube[(int)(1 + 1.5 * MySin(D3DX_PI / 4 * (2 + i)))][index][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * (2 + i)))].bottom);
	}

	rubik.degree = inverse ? 90 : -90;
	rubik.XZ = true;
	for (int i = 0; i < 3; ++i)
		for (int k = 0; k < 3; ++k)
			rubik.cube[i][index][k].rotating = true;
}

//YZ����ת��indexΪ0ʱת�����棬1ת���м䣬2ת������
void Rubik::rotate_XY(int index, bool inverse)
{
	for (int i = inverse ? 0 : 7; inverse ? (i < 6) : (i >= 2); inverse ? ++i : --i)
	{
		std::swap(rubik.cube[(int)(1 + 1.5 * MySin(D3DX_PI / 4 * i))][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * i))][index].right,
			rubik.cube[(int)(1 + 1.5 * MySin(D3DX_PI / 4 * (2 + i)))][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * (2 + i)))][index].bottom);
		std::swap(rubik.cube[(int)(1 + 1.5 * MySin(D3DX_PI / 4 * i))][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * i))][index].bottom,
			rubik.cube[(int)(1 + 1.5 * MySin(D3DX_PI / 4 * (2 + i)))][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * (2 + i)))][index].left);
		std::swap(rubik.cube[(int)(1 + 1.5 * MySin(D3DX_PI / 4 * i))][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * i))][index].left,
			rubik.cube[(int)(1 + 1.5 * MySin(D3DX_PI / 4 * (2 + i)))][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * (2 + i)))][index].top);
		std::swap(rubik.cube[(int)(1 + 1.5 * MySin(D3DX_PI / 4 * i))][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * i))][index].top,
			rubik.cube[(int)(1 + 1.5 * MySin(D3DX_PI / 4 * (2 + i)))][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * (2 + i)))][index].right);

		std::swap(rubik.cube[(int)(1 + 1.5 * MySin(D3DX_PI / 4 * i))][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * i))][index].front,
			rubik.cube[(int)(1 + 1.5 * MySin(D3DX_PI / 4 * (2 + i)))][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * (2 + i)))][index].front);
		std::swap(rubik.cube[(int)(1 + 1.5 * MySin(D3DX_PI / 4 * i))][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * i))][index].rear,
			rubik.cube[(int)(1 + 1.5 * MySin(D3DX_PI / 4 * (2 + i)))][(int)(1 + 1.5 * MyCos(D3DX_PI / 4 * (2 + i)))][index].rear);
	}

	rubik.degree = inverse ? -90 : 90;
	rubik.XY = true;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			rubik.cube[i][j][index].rotating = true;
}

//��X����ת
void Rubik::view_rotate_X(bool inverse)
{
	for (int i = 0; i < 3; ++i)
		rotate_YZ(i, inverse);
}

//��Y����ת
void Rubik::view_rotate_Y(bool inverse)
{
	for (int i = 0; i < 3; ++i)
		rotate_XZ(i, inverse);
}

//��Z����ת
void Rubik::view_rotate_Z(bool inverse)
{
	for (int i = 0; i < 3; ++i)
		rotate_XY(i, inverse);
}

