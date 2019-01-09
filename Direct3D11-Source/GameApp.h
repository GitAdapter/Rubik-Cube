#ifndef GAMEAPP_H
#define GAMEAPP_H

#include "d3dApp.h"
#include "Rubik.h"
#include "Camera.h"

class GameApp : public D3DApp
{
public:
	GameApp(HINSTANCE hInstance);
	~GameApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();


private:
	bool InitResource();

	void KeyInput();
	void MouseInput(float dt);

private:
	ComPtr<ID2D1SolidColorBrush> mColorBrush;	// ��ɫ��ˢ
	ComPtr<IDWriteFont> mFont;					// ����
	ComPtr<IDWriteTextFormat> mTextFormat;		// �ı���ʽ

	Rubik mRubik;								// ħ��

	//
	// ����������
	//
	
	int mClickPosX, mClickPosY;					// ���ε��ʱ���λ��
	float mSlideDelay;							// �϶��ӳ���Ӧʱ�� 
	float mCurrDelay;							// ��ǰ�ӳ�ʱ��
	bool mDirectionLocked;						// ������
	RubikRotationAxis mCurrRotationAxis;		// ��ǰ����϶�ʱ����ת��
	int mSlidePos;								// ��ǰ����϶��Ĳ���������-1Ϊ����ħ��

	std::unique_ptr<Camera> mCamera;			// �����˳������

	BasicEffect mBasicEffect;					// ������Ч������
};


#endif