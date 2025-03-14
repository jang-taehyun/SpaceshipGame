#pragma once

class ColorClass;

class D3DClass
{
private:
	D3DClass();
	D3DClass(const D3DClass& other);
	~D3DClass();

public:
	// singleton ��ü ȣ�� �Լ�
	inline static D3DClass* GetD3DClassInst()
	{
		if (!inst)
			inst = new D3DClass;

		return inst;
	}

	// D3D ��ü �ʱ�ȭ �Լ� //
	HRESULT Initialize(const int& ScreenWidth, const int& ScreenHeight, const bool& VSYNC, const HWND& hwnd, const bool& FullScreen, const float& ScreenDepth, const float& ScreenNear);
	
	// D3D ��ü ���� ���ҽ� ���� �Լ� //
	void Shutdown();

	// back buffer�� ����� �Լ� //
	void BeginScene(const ColorClass& color);
	
	// ȭ�鿡 back buffer�� ������ ǥ���ϴ� �Լ�
	void EndScene();

	// Getter //

	inline ID3D11Device* const& GetDevice() const { return m_Device; }
	inline ID3D11DeviceContext* const& GetDeviceContext() const { return m_DeviceContext; }
	inline void GetVideoCardInfo(char* GraphicCardName, int& Memory)
	{
		strcpy_s(GraphicCardName, 128, m_VideoCardDescription);
		Memory = m_VideoCardMemory;
	}

	// Setter //

	inline void GetProjectionMatrix(DirectX::XMMATRIX& ProjectionMatrix) { ProjectionMatrix = m_ProjectionMatrix; }
	inline void GetWorldMatrix(DirectX::XMMATRIX& WorldMatrix) { WorldMatrix = m_WorlMatrix; }
	inline void GetOrthoMatrix(DirectX::XMMATRIX& OrthoMatrix) { OrthoMatrix = m_OrthoMatrix; }


	void TurnDepthBufferOn();
	void TurnDepthBufferOff();

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

private:
	// ������ ���÷��� ��带 ã�� �Լ�
	HRESULT GetRefreshRate(const int& ScreenWidth, const int& ScreenHeight, int& Numerator, int & Denominator);

	// Swap chain ���� �� Swap chain, Device, Device context �����ϴ� �Լ�
	HRESULT CreateSwapChainDeviceDeviceContext(const int& ScreenWidth, const int& ScreenHeight, const int& Numerator, const int& Denominator, const HWND& hwnd, const bool& FullScreen);

	// Render target view ���� �� �����ϴ� �Լ�
	HRESULT SetAndCreateRenderTargetView();

	// Depth, Stencil ���� �Լ�
	HRESULT SetDepthAndStencil(const int& ScreenWidth, const int& ScreenHeight);

	// Rasterizer ���� �Լ�
	HRESULT SetRasterizer();

	// �������� ���� Viewport ���� �Լ�
	void SetViewport(const int& ScreenWidth, const int& ScreenHeight);

	// Matrix ���� �Լ�
	void SetMatrix(const int& ScreenWidth, const int& ScreenHeight, const float& ScreenDepth, const float& ScreenNear);

	// alpha blending state ���� �Լ�
	HRESULT SetAlphaBlendState();

	// video card ������ GPU�κ��� �޾ƿ��� �Լ�
	HRESULT GetVideoCardDescription(IDXGIAdapter* const& Adapter);

private:
	static D3DClass* inst;

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