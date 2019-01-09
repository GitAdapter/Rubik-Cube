#include "GameApp.h"
#include "d3dUtil.h"
using namespace DirectX;
using namespace std::experimental;

GameApp::GameApp(HINSTANCE hInstance)
	: D3DApp(hInstance)
{
}

GameApp::~GameApp()
{
}

bool GameApp::Init()
{
	if (!D3DApp::Init())
		return false;

	if (!mBasicEffect.InitAll(md3dDevice))
		return false;

	if (!InitResource())
		return false;

	// ��ʼ����꣬���̲���Ҫ
	mMouse->SetWindow(mhMainWnd);
	mMouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);

	// ��ʼ�������ӳ�ʱ��
	mSlideDelay = 0.05f;

	return true;
}

void GameApp::OnResize()
{
	assert(md2dFactory);
	assert(mdwriteFactory);
	// �ͷ�D2D�������Դ
	mColorBrush.Reset();
	md2dRenderTarget.Reset();

	D3DApp::OnResize();

	// ΪD2D����DXGI������ȾĿ��
	ComPtr<IDXGISurface> surface;
	HR(mSwapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(surface.GetAddressOf())));
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
	HRESULT hr = md2dFactory->CreateDxgiSurfaceRenderTarget(surface.Get(), &props, md2dRenderTarget.GetAddressOf());
	surface.Reset();

	if (hr == E_NOINTERFACE)
	{
		OutputDebugString(L"\n���棺Direct2D��Direct3D�������Թ������ޣ��㽫�޷������ı���Ϣ�����ṩ������ѡ������\n"
			"1. ����Win7ϵͳ����Ҫ������Win7 SP1������װKB2670838������֧��Direct2D��ʾ��\n"
			"2. �������Direct3D 10.1��Direct2D�Ľ�����������ģ�"
			"https://docs.microsoft.com/zh-cn/windows/desktop/Direct2D/direct2d-and-direct3d-interoperation-overview""\n"
			"3. ʹ�ñ������⣬����FreeType��\n\n");
	}
	else if (hr == S_OK)
	{
		// �����̶���ɫˢ���ı���ʽ
		HR(md2dRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::White),
			mColorBrush.GetAddressOf()));
		HR(mdwriteFactory->CreateTextFormat(L"����", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20, L"zh-cn",
			mTextFormat.GetAddressOf()));
	}
	else
	{
		// �����쳣����
		assert(md2dRenderTarget);
	}

	// ����������ʾ
	if (mCamera != nullptr)
	{
		mCamera->SetFrustum(XM_PI * 0.4f, AspectRatio(), 1.0f, 1000.0f);
		mCamera->SetViewPort(0.0f, 0.0f, (float)mClientWidth, (float)mClientHeight);
		mBasicEffect.SetProjMatrix(mCamera->GetProjXM());
	}
}

void GameApp::UpdateScene(float dt)
{
	KeyInput(dt);
	MouseInput(dt);
	

	// ������ת
	static float theta = XM_PIDIV2;
	if (!mRubik.IsLocked())
	{
		theta *= -1.0f;
	}
	// ����ڳ���Ҳ����������(ֻ��δ������֡�Ż���Ч�õ���)
	//mRubik.RotateZ(theta);
	//// �����Ҳ���ᱻ����
	//mRubik.RotateX(2, theta);
	//mRubik.RotateY(0, theta);
	
	// ����ħ��
	mRubik.Update(dt);

}

void GameApp::DrawScene()
{
	assert(md3dImmediateContext);
	assert(mSwapChain);

	// ʹ��ƫ��ɫ�Ĵ�ɫ����
	float backgroundColor[4] = { 0.45882352f, 0.42745098f, 0.51372549f, 1.0f };
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView.Get(), backgroundColor);
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	// ����ħ��
	mRubik.Draw(md3dImmediateContext, mBasicEffect);


	//
	// ����Direct2D����
	//
	if (md2dRenderTarget != nullptr)
	{
		md2dRenderTarget->BeginDraw();

		// ����Debug���
		Mouse::State mouseState = mMouse->GetState();
		std::wstring wstr = L"���λ�ã�(" + std::to_wstring(mouseState.x) + L", " + std::to_wstring(mouseState.y) + L")";
		Ray ray = Ray::ScreenToRay(*mCamera, (float)mouseState.x, (float)mouseState.y);
		float dist;
		XMINT3 pos = mRubik.HitCube(ray, &dist);
		wstr += L"\nѡ�з���������[" + std::to_wstring(pos.x) + L"][" + std::to_wstring(pos.y) + L"][" + std::to_wstring(pos.z) + L"]\n"
			"���е㣺";
		if (dist == 0.0f)
		{
			wstr += L"��";
		}
		else
		{
			wstr += L"(" + std::to_wstring(ray.origin.x + dist * ray.direction.x) + L", " +
				std::to_wstring(ray.origin.y + dist * ray.direction.y) + L", " +
				std::to_wstring(ray.origin.z + dist * ray.direction.z) + L")";
		}

		md2dRenderTarget->DrawTextW(wstr.c_str(), (UINT)wstr.size(), mTextFormat.Get(),
			D2D1_RECT_F{ 0.0f, 0.0f, 600.0f, 200.0f }, mColorBrush.Get());
		HR(md2dRenderTarget->EndDraw());
	}

	HR(mSwapChain->Present(0, 0));
}



bool GameApp::InitResource()
{
	// ��ʼ��ħ��
	mRubik.InitResources(md3dDevice, md3dImmediateContext);
	mRubik.SetRotationSpeed(XM_2PI * 1.5f);
	// ��ʼ����Ч
	mBasicEffect.SetRenderDefault(md3dImmediateContext);
	// ��ʼ�������
	mCamera.reset(new ThirdPersonCamera);
	auto camera3rd = dynamic_cast<ThirdPersonCamera*>(mCamera.get());
	camera3rd->SetDistance(10.0f);
	camera3rd->SetDistanceMinMax(10.0f, 20.0f);
	camera3rd->SetFrustum(XM_PI * 0.4f, AspectRatio(), 1.0f, 1000.0f);
	camera3rd->SetViewPort(0.0f, 0.0f, (float)mClientWidth, (float)mClientHeight);
	camera3rd->SetTarget(XMFLOAT3());
	mBasicEffect.SetProjMatrix(camera3rd->GetProjXM());
	mBasicEffect.SetTextureArray(mRubik.GetTexArray());
	

	return true;
}

void GameApp::KeyInput(float dt)
{
	Keyboard::State keyState = mKeyboard->GetState();
	mKeyboardTracker.Update(keyState);

	//
	// ����ħ����ת
	//

	// ��ʽx
	if (mKeyboardTracker.IsKeyPressed(Keyboard::Up))
		mRubik.RotateX(XM_PIDIV2);
	// ��ʽx'
	if (mKeyboardTracker.IsKeyPressed(Keyboard::Down))
		mRubik.RotateX(-XM_PIDIV2);
	// ��ʽy
	if (mKeyboardTracker.IsKeyPressed(Keyboard::Left))
		mRubik.RotateY(XM_PIDIV2);
	// ��ʽy'
	if (mKeyboardTracker.IsKeyPressed(Keyboard::Right))
		mRubik.RotateY(-XM_PIDIV2);
	// ��ʽz'
	if (mKeyboardTracker.IsKeyPressed(Keyboard::PageUp))
		mRubik.RotateZ(XM_PIDIV2);
	// ��ʽz
	if (mKeyboardTracker.IsKeyPressed(Keyboard::PageDown))
		mRubik.RotateZ(-XM_PIDIV2);

	//
	// ������ת
	//

	// ��ʽR
	if (mKeyboardTracker.IsKeyPressed(Keyboard::I))
		mRubik.RotateX(2, XM_PIDIV2);
	// ��ʽR'
	if (mKeyboardTracker.IsKeyPressed(Keyboard::K))
		mRubik.RotateX(2, -XM_PIDIV2);
	// ��ʽU
	if (mKeyboardTracker.IsKeyPressed(Keyboard::J))
		mRubik.RotateY(2, XM_PIDIV2);
	// ��ʽU'
	if (mKeyboardTracker.IsKeyPressed(Keyboard::L))
		mRubik.RotateY(2, -XM_PIDIV2);
	// ��ʽF'
	if (mKeyboardTracker.IsKeyPressed(Keyboard::U))
		mRubik.RotateZ(0, XM_PIDIV2);
	// ��ʽF
	if (mKeyboardTracker.IsKeyPressed(Keyboard::O))
		mRubik.RotateZ(0, -XM_PIDIV2);

	// ��ʽL'
	if (mKeyboardTracker.IsKeyPressed(Keyboard::W))
		mRubik.RotateX(0, XM_PIDIV2);
	// ��ʽL
	if (mKeyboardTracker.IsKeyPressed(Keyboard::S))
		mRubik.RotateX(0, -XM_PIDIV2);
	// ��ʽD'
	if (mKeyboardTracker.IsKeyPressed(Keyboard::A))
		mRubik.RotateY(0, XM_PIDIV2);
	// ��ʽD
	if (mKeyboardTracker.IsKeyPressed(Keyboard::D))
		mRubik.RotateY(0, -XM_PIDIV2);
	// ��ʽB
	if (mKeyboardTracker.IsKeyPressed(Keyboard::Q))
		mRubik.RotateZ(2, XM_PIDIV2);
	// ��ʽB'
	if (mKeyboardTracker.IsKeyPressed(Keyboard::E))
		mRubik.RotateZ(2, -XM_PIDIV2);

	// ��ʽM
	if (mKeyboardTracker.IsKeyPressed(Keyboard::T))
		mRubik.RotateX(1, XM_PIDIV2);
	// ��ʽM'
	if (mKeyboardTracker.IsKeyPressed(Keyboard::G))
		mRubik.RotateX(1, -XM_PIDIV2);
	// ��ʽE
	if (mKeyboardTracker.IsKeyPressed(Keyboard::F))
		mRubik.RotateY(1, XM_PIDIV2);
	// ��ʽE'
	if (mKeyboardTracker.IsKeyPressed(Keyboard::H))
		mRubik.RotateY(1, -XM_PIDIV2);
	// ��ʽS'
	if (mKeyboardTracker.IsKeyPressed(Keyboard::R))
		mRubik.RotateZ(1, XM_PIDIV2);
	// ��ʽS
	if (mKeyboardTracker.IsKeyPressed(Keyboard::Y))
		mRubik.RotateZ(1, -XM_PIDIV2);

	
}

void GameApp::MouseInput(float dt)
{
	Mouse::State mouseState = mMouse->GetState();
	Mouse::State lastState = mMouseTracker.GetLastState();
	mMouseTracker.Update(mouseState);
	

	int dx = mouseState.x - lastState.x;
	int dy = mouseState.y - lastState.y;
	// ��ȡ����
	auto cam3rd = dynamic_cast<ThirdPersonCamera*>(mCamera.get());

	// ******************
	// �����˳�������Ĳ���
	//

	// ��������ת�������΢����
	cam3rd->SetRotationX(XM_PIDIV2 * 0.6f + (mouseState.y - mClientHeight / 2) *  0.0001f);
	cam3rd->SetRotationY(-XM_PIDIV4 + (mouseState.x - mClientWidth / 2) * 0.0001f);
	cam3rd->Approach(-mouseState.scrollWheelValue / 120 * 1.0f);

	// ���¹۲����
	mCamera->UpdateViewMatrix();
	mBasicEffect.SetViewMatrix(mCamera->GetViewXM());

	// ���ù���ֵ
	mMouse->ResetScrollWheelValue();

	// ******************
	// ħ������
	//

	// �������Ƿ���
	if (mouseState.leftButton)
	{
		// ��ʱδȷ����ת����
		if (!mDirectionLocked)
		{
			// ��ʱδ��¼���λ��
			if (mClickPosX == -1 && mClickPosY == -1)
			{
				// ���ε��
				if (mMouseTracker.leftButton == Mouse::ButtonStateTracker::PRESSED)
				{
					// ��¼���λ��
					mClickPosX = mouseState.x;
					mClickPosY = mouseState.y;
				}
			}
			
			// ������¼�˵��λ�òŽ��и���
			if (mClickPosX != -1 && mClickPosY != -1)
				mCurrDelay += dt;
			// δ���ﻬ���ӳ�ʱ�������
			if (mCurrDelay < mSlideDelay)
				return;

			// δ�����˶�������
			if (abs(dx) == abs(dy))
				return;

			// ��ʼ�Ϸ�����
			mDirectionLocked = true;
			// �����ۻ���λ�Ʊ仯��
			dx = mouseState.x - mClickPosX;
			dy = mouseState.y - mClickPosY;

			// �ҵ���ǰ������ķ�������
			Ray ray = Ray::ScreenToRay(*mCamera, (float)mouseState.x, (float)mouseState.y);
			float dist;
			XMINT3 pos = mRubik.HitCube(ray, &dist);

			// �жϵ�ǰ��Ҫ�Ǵ�ֱ��������ˮƽ����
			bool isVertical = abs(dx) < abs(dy);
			// ��ǰ�����ݵ���-Z�棬���ݲ������;�����ת��
			if (pos.z == 0 && fabs((ray.origin.z + dist * ray.direction.z) - (-3.0f)) < 1e-5f)
			{
				mSlidePos = isVertical ? pos.x : pos.y;
				mCurrRotationAxis = isVertical ? RubikRotationAxis_X : RubikRotationAxis_Y;
			}
			// ��ǰ�����ݵ���+X�棬���ݲ������;�����ת��
			else if (pos.x == 2 && fabs((ray.origin.x + dist * ray.direction.x) - 3.0f) < 1e-5f)
			{
				mSlidePos = isVertical ? pos.z : pos.y;
				mCurrRotationAxis = isVertical ? RubikRotationAxis_Z : RubikRotationAxis_Y;
			}
			// ��ǰ�����ݵ���+Y�棬Ҫ�ж�ƽ�Ʊ仯��dx��dy�ķ�����������ת����
			else if (pos.y == 2 && fabs((ray.origin.y + dist * ray.direction.y) - 3.0f) < 1e-5f)
			{
				// �ж����
				bool diffSign = ((dx & 0x80000000) != (dy & 0x80000000));
				mSlidePos = diffSign ? pos.x : pos.z;
				mCurrRotationAxis = diffSign ? RubikRotationAxis_X : RubikRotationAxis_Z;
			}
			// ��ǰ�����ݵ��ǿհ׵������������ħ����ת
			else
			{
				mSlidePos = -1;
				// ˮƽ������Y����ת
				if (!isVertical)
				{
					mCurrRotationAxis = RubikRotationAxis_Y;
				}
				// ��Ļ��벿�ֵĴ�ֱ������X����ת
				else if (mouseState.x < mClientWidth / 2)
				{
					mCurrRotationAxis = RubikRotationAxis_X;
				}
				// ��Ļ�Ұ벿�ֵĴ�ֱ������Z����ת
				else
				{
					mCurrRotationAxis = RubikRotationAxis_Z;
				}
			}
		}

		// ���˷��������ܽ�����ת
		if (mDirectionLocked)
		{
			// ������ת
			if (mSlidePos == -1)
			{
				switch (mCurrRotationAxis)
				{
				case RubikRotationAxis_X: mRubik.RotateX((dx - dy) * 0.01f, true); break;
				case RubikRotationAxis_Y: mRubik.RotateY(-dx * 0.01f, true); break;
				case RubikRotationAxis_Z: mRubik.RotateZ((-dx - dy) * 0.01f, true); break;
				}
			}
			else
			{
				switch (mCurrRotationAxis)
				{
				case RubikRotationAxis_X: mRubik.RotateX(mSlidePos, (dx - dy) * 0.01f, true); break;
				case RubikRotationAxis_Y: mRubik.RotateY(mSlidePos, -dx * 0.01f, true); break;
				case RubikRotationAxis_Z: mRubik.RotateZ(mSlidePos, (-dx - dy) * 0.01f, true); break;
				}

			}
		}
	}
	// �����ͷ�
	else if (mMouseTracker.leftButton == Mouse::ButtonStateTracker::RELEASED)
	{
		// �ͷŷ�����
		mDirectionLocked = false;
		// �����ӳٹ���
		mCurrDelay = 0.0f;
		// �����Ƴ���Ļ
		mClickPosX = mClickPosY = -1;
		// �������ָ�����Ԥ��ת
		if (mSlidePos == -1)
		{
			switch (mCurrRotationAxis)
			{
			case RubikRotationAxis_X: mRubik.RotateX(0.0f); break;
			case RubikRotationAxis_Y: mRubik.RotateY(0.0f); break;
			case RubikRotationAxis_Z: mRubik.RotateZ(0.0f); break;
			}
		}
		else
		{
			switch (mCurrRotationAxis)
			{
			case RubikRotationAxis_X: mRubik.RotateX(mSlidePos, 0.0f); break;
			case RubikRotationAxis_Y: mRubik.RotateY(mSlidePos, 0.0f); break;
			case RubikRotationAxis_Z: mRubik.RotateZ(mSlidePos, 0.0f); break;
			}

		}
	}
}

