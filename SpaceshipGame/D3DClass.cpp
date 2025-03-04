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

	// 1. DirectX Graphics Infrastructure(DXGI)를 통해 적절한 디스플레이 모드 찾기 및 적용
	if (FAILED(GetRefreshRate(ScreenWidth, ScreenHeight, Numerator, Denominator)))
		return E_FAIL;

	// 2. Swap chain 설정 및 Swap chain, Device, Device context 생성
	if (FAILED(CreateSwapChainDeviceDeviceContext(ScreenWidth, ScreenHeight, Numerator, Denominator, hwnd, FullScreen)))
		return E_FAIL;

	// 3. Render target view 생성 및 설정
	if (FAILED(SetAndCreateRenderTargetView()))
		return E_FAIL;

	// 4. Depth Stencil buffer 생성
	if (FAILED(SetDepthAndStencil(ScreenWidth, ScreenHeight)))
		return E_FAIL;

	// 5. Rasterizer 설정
	if (FAILED(SetRasterizer()))
		return E_FAIL;
	
	// 6. 렌더링을 위한 Viewport 설정
	SetViewport(ScreenWidth, ScreenHeight);
	
	// 7. Matrix 설정
	SetMatrix(ScreenWidth, ScreenHeight, ScreenDepth, ScreenNear);

	// 8. alpha blending state 설정
	if (FAILED(SetAlphaBlendState()))
		return E_FAIL;

	return S_OK;
}

void D3DClass::Shutdown()
{
	// full screen 모드인 경우, 윈도우 모드로 변경
	// swap chain 해제 시에, 윈도우 모드로 설정하지 않으면 해제할 때 예외가 발생
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
	// back buffer를 지울 색상 설정
	float color[4] = { red, green, blue, alpha };

	// back buffer 초기화
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, color);

	// depth buffer 초기화
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);
}

void D3DClass::EndScene()
{
	if (m_VSYNC_Enabled)
	{
		// FPS를 고정
		m_SwapChain->Present(1, 0);
	}
	else
	{
		// 가능한 빠르게 출력
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
	// blend factor를 설정하고, alpha blending state 활성화 //
	float BlendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
	m_DeviceContext->OMSetBlendState(m_AlphaEnableBlendingState, BlendFactor, 0xffffffff);
}

void D3DClass::TurnOffAlphaBlending()
{
	// blend factor를 설정하고, alpha blending state 비활성화 //
	float BlendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
	m_DeviceContext->OMSetBlendState(m_AlphaDisableBlendingState, BlendFactor, 0xffffffff);
}

HRESULT D3DClass::GetRefreshRate(const int& ScreenWidth, const int& ScreenHeight, int& Numerator, int& Denominator)
{
	// DXGI factory 생성
	IDXGIFactory* Factory = nullptr;
	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&Factory)))
		return E_FAIL;

	// 기본 그래픽카드 및 기본 모니터 조회
	// 기본 그래픽카드
	IDXGIAdapter* Adapter = nullptr;
	if (FAILED(Factory->EnumAdapters(0, &Adapter)))
		return E_FAIL;
	// 기본 모니터
	IDXGIOutput* AdapterOutput = nullptr;
	if (FAILED(Adapter->EnumOutputs(0, &AdapterOutput)))
		return E_FAIL;

	// 모니터에서 DXGI_FORMAT_R8G8B8A8_UNORM 표시 형식에 맞는 모드 수 조회
	unsigned int ModeCount = 0;
	if (FAILED(AdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &ModeCount, NULL)))
		return E_FAIL;

	// 디스플레이 모드에 대한 모든 조합을 구하기
	DXGI_MODE_DESC* DisplayModeList = new DXGI_MODE_DESC[ModeCount];
	if (!DisplayModeList)
		return E_FAIL;
	if (FAILED(AdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &ModeCount, DisplayModeList)))
		return E_FAIL;

	// 모든 조합에서 윈도우의 가로, 세로 길이에 맞는 디스플레이 모드를 찾고,
	// FPS의 분모, 분자 값 저장
	for (unsigned int i = 0; i < ModeCount; ++i)
	{
		if (DisplayModeList[i].Width == (unsigned int)ScreenWidth && DisplayModeList[i].Height == (unsigned int)ScreenHeight)
		{
			Numerator = DisplayModeList[i].RefreshRate.Numerator;
			Denominator = DisplayModeList[i].RefreshRate.Denominator;
		}
	}

	// 그래픽카드 정보 가져오기
	if (FAILED(GetVideoCardDescription(Adapter)))
		return E_FAIL;

	// 디스플레이 모드 리스트, adapter, factory 객체 해제 //
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

	// swap chain 설정
	SwapChainDesc.BufferCount = 1;									// back buffer의 개수
	SwapChainDesc.BufferDesc.Width = ScreenWidth;					// back buffer의 해상도
	SwapChainDesc.BufferDesc.Height = ScreenHeight;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// back buffer의 format

	// swap chain의 refresh rate 설정
	if (m_VSYNC_Enabled)
	{
		// refresh rate 고정
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = Numerator;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = Denominator;
	}
	else
	{
		// 최대한 빠르게
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// back buffer의 용도
	SwapChainDesc.OutputWindow = hwnd;								// 렌더링에 사용될 윈도우 핸들

	SwapChainDesc.SampleDesc.Count = 1;								// 멀티샘플링 설정(OFF)
	SwapChainDesc.SampleDesc.Quality = 0;

	// swap chain의 디스플레이 모드 설정
	if (FullScreen)
		SwapChainDesc.Windowed = false;
	else
		SwapChainDesc.Windowed = true;

	// swap chain의 스캔 라인 설정
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// swap chain이 출력된 이후 설정(back buffer clear)
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// swap chain 추가 flag 설정
	SwapChainDesc.Flags = 0;

	// DirectX의 version 설정
	D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;

	// swap chain, device, device context 생성
	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &FeatureLevel, 1, D3D11_SDK_VERSION, &SwapChainDesc, &m_SwapChain, &m_Device, NULL, &m_DeviceContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT D3DClass::SetAndCreateRenderTargetView()
{
	// back buffer의 pointer 가져오기
	ID3D11Texture2D* BackBufferPtr = nullptr;
	if (FAILED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBufferPtr)))
		return E_FAIL;

	// render target view를 생성하고, render target view가 back buffer를 가르키도록 설정
	if (FAILED(m_Device->CreateRenderTargetView(BackBufferPtr, NULL, &m_RenderTargetView)))
		return E_FAIL;

	// back buffer 해제
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

	// depth buffer 설정
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

	// depth buffer를 texture 형식으로 생성
	if (FAILED(m_Device->CreateTexture2D(&DepthBufferDesc, NULL, &m_DepthStencilBuffer)))
		return E_FAIL;

	// depth stencil state 설정(depth buffer를 활성화한 depth stencil state)
	DepthStencilDesc.DepthEnable = true;
	DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	DepthStencilDesc.StencilEnable = true;
	DepthStencilDesc.StencilReadMask = 0xFF;
	DepthStencilDesc.StencilWriteMask = 0xFF;

	// pixel 정면의 stencil 설정
	DepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	DepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// pixel 뒷면의 stencil 설정
	DepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	DepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// depth stencil state 생성(depth buffer를 활성화한 depth stencil state)
	if (FAILED(m_Device->CreateDepthStencilState(&DepthStencilDesc, &m_DepthStencilState)))
		return E_FAIL;

	// depth stencil state 설정(depth buffer를 비활성화한 depth stencil state)
	DepthStencilDesc.DepthEnable = false;

	// depth stencil state 생성(depth buffer를 비활성화한 depth stencil state)
	if (FAILED(m_Device->CreateDepthStencilState(&DepthStencilDesc, &m_DepthDisabledStencilState)))
		return E_FAIL;

	// depth stencil state(depth buffer를 활성화한 depth stencil state)를 device context에 붙이기
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 1);

	// depth stencil view 설정
	DepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDesc.Texture2D.MipSlice = 0;

	// depth stencil view 생성
	if (FAILED(m_Device->CreateDepthStencilView(m_DepthStencilBuffer, &DepthStencilViewDesc, &m_DepthStencilView)))
		return E_FAIL;

	// render target view와 depth stencil view를 출력 렌더링 파이프라인에 바인드
	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

	return S_OK;
}

HRESULT D3DClass::SetRasterizer()
{
	D3D11_RASTERIZER_DESC RasterizerDesc;
	memset(&RasterizerDesc, 0, sizeof(D3D11_RASTERIZER_DESC));

	// Rasterizer 정보 입력
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

	// Rasterizer state 생성
	if (FAILED(m_Device->CreateRasterizerState(&RasterizerDesc, &m_RasterizerState)))
		return E_FAIL;

	// Device context에서 Rasterizer state를 설정
	m_DeviceContext->RSSetState(m_RasterizerState);

	return S_OK;
}

void D3DClass::SetViewport(const int& ScreenWidth, const int& ScreenHeight)
{
	D3D11_VIEWPORT Viewport;

	// Viewport 정보 설정
	Viewport.Width = (float)ScreenWidth;
	Viewport.Height = (float)ScreenHeight;
	Viewport.MinDepth = 0.f;
	Viewport.MaxDepth = 1.f;
	Viewport.TopLeftX = 0.f;
	Viewport.TopLeftY = 0.f;

	// Viewport 생성
	m_DeviceContext->RSSetViewports(1, &Viewport);
}

void D3DClass::SetMatrix(const int& ScreenWidth, const int& ScreenHeight, const float& ScreenDepth, const float& ScreenNear)
{
	// Projection matrix의 시야각(Field of view), 화면 비율(aspect) 설정
	float FieldOfView = DirectX::XM_PI / 4.f;
	float ScreenAspect = (float)ScreenWidth / (float)ScreenHeight;

	// Projection matrix 생성
	m_ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(FieldOfView, ScreenAspect, ScreenNear, ScreenDepth);

	// World matrix을 항등 행렬로 생성
	m_WorlMatrix = DirectX::XMMatrixIdentity();

	// Ortho matrix(직교 투영 행렬) 생성
	m_OrthoMatrix = DirectX::XMMatrixOrthographicLH((float)ScreenWidth, (float)ScreenHeight, ScreenNear, ScreenDepth);
}

HRESULT D3DClass::SetAlphaBlendState()
{
	D3D11_BLEND_DESC AlphaBlendStateDesc;
	memset(&AlphaBlendStateDesc, 0, sizeof(AlphaBlendStateDesc));

	// alpha blend state 정보 입력
	AlphaBlendStateDesc.RenderTarget[0].BlendEnable = TRUE;
	AlphaBlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	AlphaBlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	AlphaBlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	AlphaBlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	AlphaBlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	AlphaBlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	AlphaBlendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// alpha blend state 생성(blend 활성화)
	if (FAILED(m_Device->CreateBlendState(&AlphaBlendStateDesc, &m_AlphaEnableBlendingState)))
	{
		return E_FAIL;
	}

	// alpha blend state 생성(blend 비활성화)
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

	// 그래픽카드 메모리를 MB 단위로 저장
	m_VideoCardMemory = (int)(AdapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// 그래픽카드 정보 가져오기
	size_t StringLength = 0;
	if (wcstombs_s(&StringLength, m_VideoCardDescription, 128, AdapterDesc.Description, 128))
		return E_FAIL;

	return S_OK;
}
