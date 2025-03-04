#include "pch.h"
#include "D3DClass.h"

D3DClass* D3DClass::inst = nullptr;

D3DClass::D3DClass() {}
D3DClass::D3DClass(const D3DClass& other) {}
D3DClass::~D3DClass() {}

HRESULT D3DClass::Initialize(const int& const ScreenWidth, const int& const ScreenHeight, const bool& const VSYNC, const HWND& const hwnd, const bool& const FullScreen, const float& const ScreenDepth, const float& const ScreenNear)
{
	m_VSYNC_Enabled = VSYNC;

	int Numerator = 0;
	int	Denominator = 0;

	// 1. DirectX Graphics Infrastructure(DXGI)�� ���� ������ ���÷��� ��� ã�� �� ����
	if (FAILED(GetRefreshRate(ScreenWidth, ScreenHeight, Numerator, Denominator)))
		return E_FAIL;

	// 2. Swap chain ���� �� Swap chain, Device, Device context ����
	if (FAILED(CreateSwapChainDeviceDeviceContext(ScreenWidth, ScreenHeight, Numerator, Denominator, hwnd, FullScreen)))
		return E_FAIL;

	// 3. Render target view ���� �� ����
	if (FAILED(SetAndCreateRenderTargetView()))
		return E_FAIL;

	// 4. Depth Stencil buffer ����
	if (FAILED(SetDepthAndStencil(ScreenWidth, ScreenHeight)))
		return E_FAIL;

	// 5. Rasterizer ����
	if (FAILED(SetRasterizer()))
		return E_FAIL;
	
	// 6. �������� ���� Viewport ����
	SetViewport(ScreenWidth, ScreenHeight);
	
	// 7. Matrix ����
	SetMatrix(ScreenWidth, ScreenHeight, ScreenDepth, ScreenNear);

	// 8. alpha blending state ����
	if (FAILED(SetAlphaBlendState()))
		return E_FAIL;

	return S_OK;
}

void D3DClass::Shutdown()
{
	// full screen ����� ���, ������ ���� ����
	// swap chain ���� �ÿ�, ������ ���� �������� ������ ������ �� ���ܰ� �߻�
	if (m_SwapChain)
		m_SwapChain->SetFullscreenState(false, NULL);

	if (m_AlphaDisableBlendingState)
	{
		m_AlphaDisableBlendingState->Release();
		m_AlphaDisableBlendingState = nullptr;
	}

	if (m_AlphaEnableBlendingState)
	{
		m_AlphaEnableBlendingState->Release();
		m_AlphaEnableBlendingState = nullptr;
	}

	if (m_RasterizerState)
	{
		m_RasterizerState->Release();
		m_RasterizerState = nullptr;
	}

	if (m_DepthStencilView)
	{
		m_DepthStencilView->Release();
		m_DepthStencilView = nullptr;
	}

	if (m_DepthDisabledStencilState)
	{
		m_DepthDisabledStencilState->Release();
		m_DepthDisabledStencilState = nullptr;
	}

	if (m_DepthStencilState)
	{
		m_DepthStencilState->Release();
		m_DepthStencilState = nullptr;
	}

	if (m_DepthStencilBuffer)
	{
		m_DepthStencilBuffer->Release();
		m_DepthStencilBuffer = nullptr;
	}

	if (m_RenderTargetView)
	{
		m_RenderTargetView->Release();
		m_RenderTargetView = nullptr;
	}

	if (m_DeviceContext)
	{
		m_DeviceContext->Release();
		m_DeviceContext = nullptr;
	}

	if (m_Device)
	{
		m_Device->Release();
		m_Device = nullptr;
	}

	if (m_SwapChain)
	{
		m_SwapChain->Release();
		m_SwapChain = nullptr;
	}
}

void D3DClass::BeginScene(const float red, const float green, const float blue, const float alpha)
{
	// back buffer�� ���� ���� ����
	float color[4] = { red, green, blue, alpha };

	// back buffer �ʱ�ȭ
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, color);

	// depth buffer �ʱ�ȭ
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);
}

void D3DClass::EndScene()
{
	if (m_VSYNC_Enabled)
	{
		// FPS�� ����
		m_SwapChain->Present(1, 0);
	}
	else
	{
		// ������ ������ ���
		m_SwapChain->Present(0, 0);
	}
}

void D3DClass::TurnDepthBufferOn()
{
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 1);
}

void D3DClass::TurnDepthBufferOff()
{
	m_DeviceContext->OMSetDepthStencilState(m_DepthDisabledStencilState, 1);
}

void D3DClass::TurnOnAlphaBlending()
{
	// blend factor�� �����ϰ�, alpha blending state Ȱ��ȭ //
	float BlendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
	m_DeviceContext->OMSetBlendState(m_AlphaEnableBlendingState, BlendFactor, 0xffffffff);
}

void D3DClass::TurnOffAlphaBlending()
{
	// blend factor�� �����ϰ�, alpha blending state ��Ȱ��ȭ //
	float BlendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
	m_DeviceContext->OMSetBlendState(m_AlphaDisableBlendingState, BlendFactor, 0xffffffff);
}

HRESULT D3DClass::GetRefreshRate(const int& ScreenWidth, const int& ScreenHeight, int& Numerator, int& Denominator)
{
	// DXGI factory ����
	IDXGIFactory* Factory = nullptr;
	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&Factory)))
		return E_FAIL;

	// �⺻ �׷���ī�� �� �⺻ ����� ��ȸ
	// �⺻ �׷���ī��
	IDXGIAdapter* Adapter = nullptr;
	if (FAILED(Factory->EnumAdapters(0, &Adapter)))
		return E_FAIL;
	// �⺻ �����
	IDXGIOutput* AdapterOutput = nullptr;
	if (FAILED(Adapter->EnumOutputs(0, &AdapterOutput)))
		return E_FAIL;

	// ����Ϳ��� DXGI_FORMAT_R8G8B8A8_UNORM ǥ�� ���Ŀ� �´� ��� �� ��ȸ
	unsigned int ModeCount = 0;
	if (FAILED(AdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &ModeCount, NULL)))
		return E_FAIL;

	// ���÷��� ��忡 ���� ��� ������ ���ϱ�
	DXGI_MODE_DESC* DisplayModeList = new DXGI_MODE_DESC[ModeCount];
	if (!DisplayModeList)
		return E_FAIL;
	if (FAILED(AdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &ModeCount, DisplayModeList)))
		return E_FAIL;

	// ��� ���տ��� �������� ����, ���� ���̿� �´� ���÷��� ��带 ã��,
	// FPS�� �и�, ���� �� ����
	for (unsigned int i = 0; i < ModeCount; ++i)
	{
		if (DisplayModeList[i].Width == (unsigned int)ScreenWidth && DisplayModeList[i].Height == (unsigned int)ScreenHeight)
		{
			Numerator = DisplayModeList[i].RefreshRate.Numerator;
			Denominator = DisplayModeList[i].RefreshRate.Denominator;
		}
	}

	// �׷���ī�� ���� ��������
	if (FAILED(GetVideoCardDescription(Adapter)))
		return E_FAIL;

	// ���÷��� ��� ����Ʈ, adapter, factory ��ü ���� //
	delete[] DisplayModeList;
	DisplayModeList = nullptr;

	AdapterOutput->Release();
	AdapterOutput = nullptr;

	Adapter->Release();
	Adapter = nullptr;

	Factory->Release();
	Factory = nullptr;

	return S_OK;
}

HRESULT D3DClass::CreateSwapChainDeviceDeviceContext(const int& ScreenWidth, const int& ScreenHeight, const int& Numerator, const int& Denominator, const HWND& hwnd, const bool& FullScreen)
{
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	memset(&SwapChainDesc, 0, sizeof(DXGI_SWAP_CHAIN_DESC));

	// swap chain ����
	SwapChainDesc.BufferCount = 1;									// back buffer�� ����
	SwapChainDesc.BufferDesc.Width = ScreenWidth;					// back buffer�� �ػ�
	SwapChainDesc.BufferDesc.Height = ScreenHeight;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// back buffer�� format

	// swap chain�� refresh rate ����
	if (m_VSYNC_Enabled)
	{
		// refresh rate ����
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = Numerator;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = Denominator;
	}
	else
	{
		// �ִ��� ������
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// back buffer�� �뵵
	SwapChainDesc.OutputWindow = hwnd;								// �������� ���� ������ �ڵ�

	SwapChainDesc.SampleDesc.Count = 1;								// ��Ƽ���ø� ����(OFF)
	SwapChainDesc.SampleDesc.Quality = 0;

	// swap chain�� ���÷��� ��� ����
	if (FullScreen)
		SwapChainDesc.Windowed = false;
	else
		SwapChainDesc.Windowed = true;

	// swap chain�� ��ĵ ���� ����
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// swap chain�� ��µ� ���� ����(back buffer clear)
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// swap chain �߰� flag ����
	SwapChainDesc.Flags = 0;

	// DirectX�� version ����
	D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;

	// swap chain, device, device context ����
	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &FeatureLevel, 1, D3D11_SDK_VERSION, &SwapChainDesc, &m_SwapChain, &m_Device, NULL, &m_DeviceContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT D3DClass::SetAndCreateRenderTargetView()
{
	// back buffer�� pointer ��������
	ID3D11Texture2D* BackBufferPtr = nullptr;
	if (FAILED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBufferPtr)))
		return E_FAIL;

	// render target view�� �����ϰ�, render target view�� back buffer�� ����Ű���� ����
	if (FAILED(m_Device->CreateRenderTargetView(BackBufferPtr, NULL, &m_RenderTargetView)))
		return E_FAIL;

	// back buffer ����
	BackBufferPtr->Release();
	BackBufferPtr = nullptr;

	return S_OK;
}

HRESULT D3DClass::SetDepthAndStencil(const int& ScreenWidth, const int& ScreenHeight)
{
	D3D11_TEXTURE2D_DESC DepthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
	
	memset(&DepthBufferDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));
	memset(&DepthStencilDesc, 0, sizeof(D3D11_DEPTH_STENCIL_DESC));
	memset(&DepthStencilViewDesc, 0, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	// depth buffer ����
	DepthBufferDesc.Width = ScreenWidth;
	DepthBufferDesc.Height = ScreenHeight;
	DepthBufferDesc.MipLevels = 1;
	DepthBufferDesc.ArraySize = 1;
	DepthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthBufferDesc.SampleDesc.Count = 1;
	DepthBufferDesc.SampleDesc.Quality = 0;
	DepthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthBufferDesc.CPUAccessFlags = 0;
	DepthBufferDesc.MiscFlags = 0;

	// depth buffer�� texture �������� ����
	if (FAILED(m_Device->CreateTexture2D(&DepthBufferDesc, NULL, &m_DepthStencilBuffer)))
		return E_FAIL;

	// depth stencil state ����(depth buffer�� Ȱ��ȭ�� depth stencil state)
	DepthStencilDesc.DepthEnable = true;
	DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	DepthStencilDesc.StencilEnable = true;
	DepthStencilDesc.StencilReadMask = 0xFF;
	DepthStencilDesc.StencilWriteMask = 0xFF;

	// pixel ������ stencil ����
	DepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	DepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// pixel �޸��� stencil ����
	DepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	DepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// depth stencil state ����(depth buffer�� Ȱ��ȭ�� depth stencil state)
	if (FAILED(m_Device->CreateDepthStencilState(&DepthStencilDesc, &m_DepthStencilState)))
		return E_FAIL;

	// depth stencil state ����(depth buffer�� ��Ȱ��ȭ�� depth stencil state)
	DepthStencilDesc.DepthEnable = false;

	// depth stencil state ����(depth buffer�� ��Ȱ��ȭ�� depth stencil state)
	if (FAILED(m_Device->CreateDepthStencilState(&DepthStencilDesc, &m_DepthDisabledStencilState)))
		return E_FAIL;

	// depth stencil state(depth buffer�� Ȱ��ȭ�� depth stencil state)�� device context�� ���̱�
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 1);

	// depth stencil view ����
	DepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDesc.Texture2D.MipSlice = 0;

	// depth stencil view ����
	if (FAILED(m_Device->CreateDepthStencilView(m_DepthStencilBuffer, &DepthStencilViewDesc, &m_DepthStencilView)))
		return E_FAIL;

	// render target view�� depth stencil view�� ��� ������ ���������ο� ���ε�
	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

	return S_OK;
}

HRESULT D3DClass::SetRasterizer()
{
	D3D11_RASTERIZER_DESC RasterizerDesc;
	memset(&RasterizerDesc, 0, sizeof(D3D11_RASTERIZER_DESC));

	// Rasterizer ���� �Է�
	RasterizerDesc.AntialiasedLineEnable = false;
	RasterizerDesc.CullMode = D3D11_CULL_BACK;
	RasterizerDesc.DepthBias = 0;
	RasterizerDesc.DepthBiasClamp = 0.0f;
	RasterizerDesc.DepthClipEnable = true;
	RasterizerDesc.FillMode = D3D11_FILL_SOLID;
	RasterizerDesc.FrontCounterClockwise = false;
	RasterizerDesc.MultisampleEnable = false;
	RasterizerDesc.ScissorEnable = false;
	RasterizerDesc.SlopeScaledDepthBias = 0.0f;

	// Rasterizer state ����
	if (FAILED(m_Device->CreateRasterizerState(&RasterizerDesc, &m_RasterizerState)))
		return E_FAIL;

	// Device context���� Rasterizer state�� ����
	m_DeviceContext->RSSetState(m_RasterizerState);

	return S_OK;
}

void D3DClass::SetViewport(const int& ScreenWidth, const int& ScreenHeight)
{
	D3D11_VIEWPORT Viewport;

	// Viewport ���� ����
	Viewport.Width = (float)ScreenWidth;
	Viewport.Height = (float)ScreenHeight;
	Viewport.MinDepth = 0.f;
	Viewport.MaxDepth = 1.f;
	Viewport.TopLeftX = 0.f;
	Viewport.TopLeftY = 0.f;

	// Viewport ����
	m_DeviceContext->RSSetViewports(1, &Viewport);
}

void D3DClass::SetMatrix(const int& ScreenWidth, const int& ScreenHeight, const float& ScreenDepth, const float& ScreenNear)
{
	// Projection matrix�� �þ߰�(Field of view), ȭ�� ����(aspect) ����
	float FieldOfView = DirectX::XM_PI / 4.f;
	float ScreenAspect = (float)ScreenWidth / (float)ScreenHeight;

	// Projection matrix ����
	m_ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(FieldOfView, ScreenAspect, ScreenNear, ScreenDepth);

	// World matrix�� �׵� ��ķ� ����
	m_WorlMatrix = DirectX::XMMatrixIdentity();

	// Ortho matrix(���� ���� ���) ����
	m_OrthoMatrix = DirectX::XMMatrixOrthographicLH((float)ScreenWidth, (float)ScreenHeight, ScreenNear, ScreenDepth);
}

HRESULT D3DClass::SetAlphaBlendState()
{
	D3D11_BLEND_DESC AlphaBlendStateDesc;
	memset(&AlphaBlendStateDesc, 0, sizeof(AlphaBlendStateDesc));

	// alpha blend state ���� �Է�
	AlphaBlendStateDesc.RenderTarget[0].BlendEnable = TRUE;
	AlphaBlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	AlphaBlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	AlphaBlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	AlphaBlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	AlphaBlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	AlphaBlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	AlphaBlendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// alpha blend state ����(blend Ȱ��ȭ)
	if (FAILED(m_Device->CreateBlendState(&AlphaBlendStateDesc, &m_AlphaEnableBlendingState)))
	{
		return E_FAIL;
	}

	// alpha blend state ����(blend ��Ȱ��ȭ)
	AlphaBlendStateDesc.RenderTarget[0].BlendEnable = FALSE;
	if (FAILED(m_Device->CreateBlendState(&AlphaBlendStateDesc, &m_AlphaDisableBlendingState)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT D3DClass::GetVideoCardDescription(IDXGIAdapter* const Adapter)
{
	DXGI_ADAPTER_DESC AdapterDesc;
	if (FAILED(Adapter->GetDesc(&AdapterDesc)))
		return E_FAIL;

	// �׷���ī�� �޸𸮸� MB ������ ����
	m_VideoCardMemory = (int)(AdapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// �׷���ī�� ���� ��������
	size_t StringLength = 0;
	if (wcstombs_s(&StringLength, m_VideoCardDescription, 128, AdapterDesc.Description, 128))
		return E_FAIL;

	return S_OK;
}
