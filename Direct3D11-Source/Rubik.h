#ifndef RUBIK_H
#define RUBIK_H

#include <wrl/client.h>
#include "Effects.h"
#include <vector>

enum RubikFaceColor {
	RubikFaceColor_Black,		// ��ɫ
	RubikFaceColor_Orange,		// ��ɫ
	RubikFaceColor_Red,			// ��ɫ
	RubikFaceColor_Green,		// ��ɫ
	RubikFaceColor_Blue,		// ��ɫ
	RubikFaceColor_Yellow,		// ��ɫ
	RubikFaceColor_White		// ��ɫ
};

enum RubikFace {
	RubikFace_PosX,		// +X��
	RubikFace_NegX,		// -X��
	RubikFace_PosY,		// +Y��
	RubikFace_NegY,		// -Y��
	RubikFace_PosZ,		// +Z��
	RubikFace_NegZ,		// -Z��
};

struct Cube
{
	// ��ȡ��ǰ��������������
	DirectX::XMMATRIX GetWorldMatrix() const;

	RubikFaceColor faceColors[6];	// ���������ɫ������0-5�ֱ��Ӧ+X, -X, +Y, -Y, +Z, -Z��
	DirectX::XMFLOAT3 pos;			// ��ת��������������λ��
	DirectX::XMFLOAT3 rotation;		// ��������ڵ�����ת����¼��ǰ�ֱ���x��, y��, z����ת�Ļ���

};


class Rubik
{
public:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	Rubik();

	// ��ʼ����Դ
	void InitResources(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	// ������ԭħ��
	void Reset();
	// ����ħ��״̬
	void Update(float dt);
	// ����ħ��
	void Draw(ComPtr<ID3D11DeviceContext> deviceContext, BasicEffect& effect);
	// ��ǰ�Ƿ��ڽ��ж�����
	bool IsLocked() const;

	// ��X����ת����ħ��
	void RotateX(float dTheta, bool isPressed = false);
	// ��X����ת����ħ��
	void RotateY(float dTheta, bool isPressed = false);
	// ��X����ת����ħ��
	void RotateZ(float dTheta, bool isPressed = false);

	// ��X����תħ��ĳһ��
	void RotateX(int pos, float dTheta, bool isPressed = false);
	// ��Y����תħ��ĳһ��
	void RotateY(int pos, float dTheta, bool isPressed = false);
	// ��Z����תħ��ĳһ��
	void RotateZ(int pos, float dTheta, bool isPressed = false);

	// ������ת�ٶ�(rad/s)
	void SetRotationSpeed(float rad);

	// ��ȡ��������
	ComPtr<ID3D11ShaderResourceView> GetTexArray() const;

private:
	// ��X���Ԥ��ת
	void PreRotateX(bool isKeyOp);
	// ��Y���Ԥ��ת
	void PreRotateY(bool isKeyOp);
	// ��Z���Ԥ��ת
	void PreRotateZ(bool isKeyOp);

	// ��ȡ��Ҫ�뵱ǰ������ֵ���н���������������ģ����ת
	// outArr1 { [X1][Y1] [X2][Y2] ... }
	//              ||       ||
	// outArr2 { [X1][Y1] [X2][Y2] ... }
	void GetSwapIndexArray(int times, std::vector<DirectX::XMINT2>& outArr1, 
		std::vector<DirectX::XMINT2>& outArr2) const;

	// ��ȡ��X����ת���������Ҫ��Ŀ�������齻�����棬����ģ����ת
	// cube[][Y][Z].face1 <--> cube[][Y][Z].face2
	RubikFace GetTargetSwapFaceRotationX(RubikFace face, int times) const;
	// ��ȡ��Y����ת���������Ҫ��Ŀ�������齻�����棬����ģ����ת
	// cube[X][][Z].face1 <--> cube[X][][Z].face2
	RubikFace GetTargetSwapFaceRotationY(RubikFace face, int times) const;
	// ��ȡ��Z����ת���������Ҫ��Ŀ�������齻�����棬����ģ����ת
	// cube[X][Y][].face1 <--> cube[X][Y][].face2
	RubikFace GetTargetSwapFaceRotationZ(RubikFace face, int times) const;

private:
	// ħ�� [X][Y][Z]
	Cube mCubes[3][3][3];

	// ��ǰ�Ƿ���������϶�
	bool mIsPressed;
	// ��ǰ�Ƿ��ж����ڲ���
	bool mIsLocked;
	// ��ǰ�Զ���ת���ٶ�
	float mRotationSpeed;

	// ���㻺����������6�����24������
	// ����0-3��Ӧ+X��
	// ����4-7��Ӧ-X��
	// ����8-11��Ӧ+Y��
	// ����12-15��Ӧ-Y��
	// ����16-19��Ӧ+Z��
	// ����20-23��Ӧ-Z��
	ComPtr<ID3D11Buffer> mVertexBuffer;	

	// ��������������6������
	ComPtr<ID3D11Buffer> mIndexBuffer;
	
	// �������飬����7������
	ComPtr<ID3D11ShaderResourceView> mTexArray;
};




#endif