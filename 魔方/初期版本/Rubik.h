#pragma once
#include "DirectX.h"

//ħ����һ��С������
struct Cube
{
	Vetrex v[24];
	LPDIRECT3DTEXTURE9 top, bottom, left, right, front, rear;
	bool rotating;
};

//ħ��
struct Rubik
{
	Cube cube[3][3][3];
	D3DXMATRIX matrix;
	int degree;			//��ת�ĽǶ�
	int shuffle_times;	//��ʼ��ʱ���Ҵ���
	bool YZ, XZ, XY;	//��ת��������
	bool complete;		//ħ���Ƿ����

	void reset();			//����
	void update();			//����
	void show();			//��ʾ
	bool is_complete(HWND window);		//�Ƿ����

	void rotate_YZ(int index, bool inverse);	//YZ����ת��indexΪ0ʱת�����棬1ת���м䣬2ת������
	void rotate_XZ(int index, bool inverse);	//XZ����ת��indexΪ0ʱת�����棬1ת���м䣬2ת������
	void rotate_XY(int index, bool inverse);	//YZ����ת��indexΪ0ʱת�����棬1ת���м䣬2ת������

	
	void view_rotate_X(bool inverse);			//��X����ת
	void view_rotate_Y(bool inverse);			//��Y����ת
	void view_rotate_Z(bool inverse);			//��Z����ת
};


