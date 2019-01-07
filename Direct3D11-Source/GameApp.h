#ifndef GAMEAPP_H
#define GAMEAPP_H

#include "d3dApp.h"
#include "Rubik.h"

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

private:
	ComPtr<ID2D1SolidColorBrush> mColorBrush;	// ��ɫ��ˢ
	ComPtr<IDWriteFont> mFont;					// ����
	ComPtr<IDWriteTextFormat> mTextFormat;		// �ı���ʽ

	Rubik mRubik;								// ħ��

	BasicEffect mBasicEffect;					// ������Ч������
};


#endif