#pragma once

class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();

	// back buffer를 지우는 함수
	void BeginScene(float, float, float, float);
	
	// 화면에 back buffer의 내용을 표시하는 함수
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(DirectX::XMMATRIX&);
	void GetWorldMatrix(DirectX::XMMATRIX&);
	void GetOrthoMatrix(DirectX::XMMATRIX&);

	void GetVideoCardInfo(char*, int&);

	void TurnDepthBufferOn();
	void TurnDepthBufferOff();

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

	// DirectX 11 초기화 단계 //
private:
	// 1. DirectX Graphics infrastructure(DXGI)를 통해 적절한 디스플레이 모드 찾기 및 적용
	bool GetRefreshRate(int ScreenWidth, int ScreenHeight, int& Numerator, int & Denominator);

	// 2. Swap chain 설정 및 Swap chain, Device, Device context 생성
	bool CreateSwapChainDeviceDeviceContext(int ScreenWidth, int ScreenHeight, int Numerator, int Denominator, HWND hwnd, bool FullScreen);

	// 3. Render target view 생성 및 설정
	bool SetAndCreateRenderTargetView();

	// 4. Depth, Stencil 설정
	bool SetDepthAndStencil(int ScreenWidth, int ScreenHeight);

	// 5. Rasterizer 설정
	bool SetRasterizer();

	// 6. 렌더링을 위한 Viewport 설정
	bool SetViewport(int ScreenWidth, int ScreenHeight);

	// 7. Matrix 설정
	bool SetMatrix(int ScreenWidth, int ScreenHeight, float ScreenDepth, float ScreenNear);

	// 8. alpha blending state 설정
	bool SetAlphaBlendState();

	// video card 정보를 GPU로부터 받아오는 함수
	bool GetVideoCardDescription(IDXGIAdapter* Adapter);

private:
	bool m_VSYNC_Enabled = false;

	// about graphics card
	int m_VideoCardMemory = 0;
	char m_VideoCardDescription[128] = { 0, };

	// swap chain
	IDXGISwapChain* m_SwapChain = nullptr;

	// device, device context, render target view
	ID3D11Device* m_Device = nullptr;
	ID3D11DeviceContext* m_DeviceContext = nullptr;
	ID3D11RenderTargetView* m_RenderTargetView = nullptr;

	// depth stencil
	ID3D11Texture2D* m_DepthStencilBuffer = nullptr;
	ID3D11DepthStencilState* m_DepthStencilState = nullptr;
	ID3D11DepthStencilState* m_DepthDisabledStencilState = nullptr;
	ID3D11DepthStencilView* m_DepthStencilView = nullptr;

	// rasterizer
	ID3D11RasterizerState* m_RasterizerState = nullptr;

	// matrix
	DirectX::XMMATRIX m_ProjectionMatrix;
	DirectX::XMMATRIX m_WorlMatrix;
	DirectX::XMMATRIX m_OrthoMatrix;

	// alpha blending
	ID3D11BlendState* m_AlphaEnableBlendingState = nullptr;
	ID3D11BlendState* m_AlphaDisableBlendingState = nullptr;
};

