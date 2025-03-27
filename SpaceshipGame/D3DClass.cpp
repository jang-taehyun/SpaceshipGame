#include "pch.h"
#include "ColorClass.h"
#include "D3DClass.h"

bool D3DClass::IsInitialize = false;

D3DClass::D3DClass(const int& ScreenWidth, const int& ScreenHeight, const bool& VSYNC, const HWND& hwnd, const bool& FullScreen, const float& ScreenDepth, const float& ScreenNear)
{
	ErrorContent e;
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("D3DClass Constructor");

	if (IsInitialize)
	{
		e.contents = _T("이미 D3DClass 인스턴스가 존재합니다.");
		e.errorCode = E_FAIL;
		throw e;
	}

	result = Initialize(ScreenWidth, ScreenHeight, VSYNC, hwnd, FullScreen, ScreenDepth, ScreenNear);
	if (FAILED(result))
	{
		Shutdown();

		e.contents = _T("D3DClass 초기화 실패");
		e.errorCode = result;
		throw e;
	}

	IsInitialize = true;
}

D3DClass::~D3DClass()
{
	Shutdown();
	IsInitialize = false;
}

HRESULT D3DClass::Initialize(const int& ScreenWidth, const int& ScreenHeight, const bool& VSYNC, const HWND& hwnd, const bool& FullScreen, const float& ScreenDepth, const float& ScreenNear)
{
	ErrorContent e;
	HRESULT result = S_OK;
	int Numerator = 0, Denominator = 0;

	// 에러 메세지 초기화 //
	e.title = _T("D3DClass Initialize()");

	m_VSYNC_Enabled = VSYNC;

	// 1. DirectX Graphics Infrastructure(DXGI)를 통해 적절한 디스플레이 모드 찾기 및 적용
	result = GetRefreshRate(ScreenWidth, ScreenHeight, Numerator, Denominator);
	if (FAILED(result))
	{
		Shutdown();

		e.contents = _T("적절한 디스플레이 모드 찾기 및 적용 실패");
		e.errorCode = result;
		throw e;
	}

	// 2. Swap chain 설정 및 Swap chain, Device, Device context 생성
	result = CreateSwapChainDeviceDeviceContext(ScreenWidth, ScreenHeight, Numerator, Denominator, hwnd, FullScreen);
	if (FAILED(result))
	{
		Shutdown();

		e.contents = _T("Swap chain 설정 및 Swap chain, Device, Device context 생성 실패");
		e.errorCode = result;
		throw e;
	}

	// 3. Render target view 생성 및 설정
	result = SetAndCreateRenderTargetView();
	if (FAILED(result))
	{
		Shutdown();

		e.contents = _T("Render target view 생성 및 설정 실패");
		e.errorCode = result;
		throw e;
	}

	// 4. Depth Stencil buffer 생성
	result = SetDepthAndStencil(ScreenWidth, ScreenHeight);
	if (FAILED(result))
	{
		Shutdown();

		e.contents = _T("Depth Stencil buffer 생성 실패");
		e.errorCode = result;
		throw e;
	}

	// 5. Rasterizer 설정
	result = SetRasterizer();
	if (FAILED(result))
	{
		Shutdown();

		e.contents = _T("Rasterizer 설정 실패");
		e.errorCode = result;
		throw e;
	}
	
	// 6. 렌더링을 위한 Viewport 설정
	SetViewport(ScreenWidth, ScreenHeight);
	
	// 7. Matrix 설정
	SetMatrix(ScreenWidth, ScreenHeight, ScreenDepth, ScreenNear);

	// 8. alpha blending state 설정
	result = SetAlphaBlendState();
	if (FAILED(result))
	{
		Shutdown();

		e.contents = _T("alpha blending state 설정 실패");
		e.errorCode = result;
		throw e;
	}

	return result;
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

void D3DClass::BeginScene(const ColorClass& color)
{
	// back buffer를 지울 색상 설정
	float background[4] = { color.GetColorRed(), color.GetColorGreen(), color.GetColorBlue(), color.GetColorAlpha() };

	// back buffer 초기화
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, background);

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
	ErrorContent e;
	HRESULT result = S_OK;
	IDXGIFactory* Factory = nullptr;			// DXGI factory
	IDXGIAdapter* Adapter = nullptr;			// 기본 그래픽 카드
	IDXGIOutput* AdapterOutput = nullptr;		// 기본 모니터
	unsigned int ModeCount = 0;					// display mode의 개수
	DXGI_MODE_DESC* DisplayModeList = nullptr;	// display mode의 정보를 담은 배열

	// 에러 메세지 초기화 //
	e.title = _T("D3DClass GetRefreshRate()");

	// DXGI factory 생성 //
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&Factory);
	if (FAILED(result))
	{
		Shutdown();

		e.contents = _T("DXGI factory 생성 실패");
		e.errorCode = result;
		throw e;
	}

	// 기본 그래픽카드 및 기본 모니터 조회 //
	// 기본 그래픽카드
	result = Factory->EnumAdapters(0, &Adapter);
	if (FAILED(result))
	{
		Shutdown();

		e.contents = _T("기본 그래픽카드 조회 실패");
		e.errorCode = result;
		throw e;
	}

	// 기본 모니터
	result = Adapter->EnumOutputs(0, &AdapterOutput);
	if (FAILED(result))
	{
		Shutdown();

		e.contents = _T("기본 모니터 조회 실패");
		e.errorCode = result;
		throw e;
	}

	// 모니터에서 DXGI_FORMAT_R8G8B8A8_UNORM 표시 형식에 맞는 모드 수 조회 //
	result = AdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &ModeCount, NULL);
	if (FAILED(result))
	{
		Shutdown();

		e.contents = _T("DXGI_FORMAT_R8G8B8A8_UNORM 표시 형식에 맞는 모드 수 조회 실패");
		e.errorCode = result;
		throw e;
	}

	// 디스플레이 모드에 대한 모든 조합을 구하기 //
	DisplayModeList = new DXGI_MODE_DESC[ModeCount];
	if (!DisplayModeList)
	{
		Shutdown();

		e.contents = _T("display mode의 정보를 담을 배열 생성 실패");
		e.errorCode = E_FAIL;
		throw e;
	}
	result = AdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &ModeCount, DisplayModeList);
	if (FAILED(result))
	{
		Shutdown();

		e.contents = _T("디스플레이 모드에 대한 모든 조합 조회 실패");
		e.errorCode = result;
		throw e;
	}

	// 모든 조합에서 윈도우의 가로, 세로 길이에 맞는 디스플레이 모드를 찾고, FPS의 분모, 분자 값 저장 //
	for (unsigned int i = 0; i < ModeCount; ++i)
	{
		if (DisplayModeList[i].Width == (unsigned int)ScreenWidth && DisplayModeList[i].Height == (unsigned int)ScreenHeight)
		{
			Numerator = DisplayModeList[i].RefreshRate.Numerator;
			Denominator = DisplayModeList[i].RefreshRate.Denominator;
		}
	}

	// 그래픽카드 정보 가져오기 //
	result = GetVideoCardDescription(Adapter);
	if (FAILED(result))
	{
		Shutdown();

		e.contents = _T("그래픽카드 정보 가져오기 실패");
		e.errorCode = result;
		throw e;
	}

	// 디스플레이 모드 리스트, adapter, factory 객체 해제 //
	delete[] DisplayModeList;
	DisplayModeList = nullptr;

	AdapterOutput->Release();
	AdapterOutput = nullptr;

	Adapter->Release();
	Adapter = nullptr;

	Factory->Release();
	Factory = nullptr;

	return result;
}

HRESULT D3DClass::CreateSwapChainDeviceDeviceContext(const int& ScreenWidth, const int& ScreenHeight, const int& Numerator, const int& Denominator, const HWND& hwnd, const bool& FullScreen)
{
	ErrorContent e;
	HRESULT result = S_OK;
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;				// swap chain 설정 정보
	D3D_FEATURE_LEVEL FeatureLevel;					// DirectX 버전 정보

	// 에러 메세지 초기화 //
	e.title = _T("D3DClass CreateSwapChainDeviceDeviceContext()");

	// swap chain 설정 //
	memset(&SwapChainDesc, 0, sizeof(DXGI_SWAP_CHAIN_DESC));
	SwapChainDesc.BufferCount = 1;									// back buffer의 개수
	SwapChainDesc.BufferDesc.Width = ScreenWidth;					// back buffer의 해상도
	SwapChainDesc.BufferDesc.Height = ScreenHeight;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// back buffer의 format

	// swap chain의 refresh rate 설정 //
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

	// DirectX의 version 설정 // 
	FeatureLevel = D3D_FEATURE_LEVEL_11_0;

	// swap chain, device, device context 생성 //
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &FeatureLevel, 1, D3D11_SDK_VERSION, &SwapChainDesc, &m_SwapChain, &m_Device, NULL, &m_DeviceContext);
	if (FAILED(result))
	{
		Shutdown();

		e.contents = _T("swap chain, device, device context 생성 실패");
		e.errorCode = result;
		throw e;
	}

	return result;
}

HRESULT D3DClass::SetAndCreateRenderTargetView()
{
	ErrorContent e;
	HRESULT result = S_OK;
	ID3D11Texture2D* BackBufferPtr = nullptr;			// back buffer의 포인터

	// 에러 메세지 초기화 //
	e.title = _T("D3DClass SetAndCreateRenderTargetView()");

	// back buffer의 pointer 가져오기 //
	result = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBufferPtr);
	if (FAILED(result))
	{
		Shutdown();

		e.contents = _T("back buffer의 pointer 가져오기 실패");
		e.errorCode = result;
		throw e;
	}

	// render target view를 생성하고, render target view가 back buffer를 가르키도록 설정 //
	result = m_Device->CreateRenderTargetView(BackBufferPtr, NULL, &m_RenderTargetView);
	if (FAILED(result))
	{
		Shutdown();

		e.contents = _T("render target view 생성, render target view가 back buffer를 가르키도록 설정 실패");
		e.errorCode = result;
		throw e;
	}

	// back buffer 해제 //
	BackBufferPtr->Release();
	BackBufferPtr = nullptr;

	return result;
}

HRESULT D3DClass::SetDepthAndStencil(const int& ScreenWidth, const int& ScreenHeight)
{
	ErrorContent e;
	HRESULT result = S_OK;
	D3D11_TEXTURE2D_DESC DepthBufferDesc;					// depth buffer 설정 정보
	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;				// depth stencil state 설정 정보
	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;		// depth stencil view 설정 정보

	// 에러 메세지 초기화 //
	e.title = _T("D3DClass SetDepthAndStencil()");

	memset(&DepthBufferDesc, 0, sizeof(DepthBufferDesc));
	memset(&DepthStencilDesc, 0, sizeof(DepthStencilDesc));
	memset(&DepthStencilViewDesc, 0, sizeof(DepthStencilViewDesc));

	// depth buffer 설정 //
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

	// depth buffer를 texture 형식으로 생성 //
	result = m_Device->CreateTexture2D(&DepthBufferDesc, NULL, &m_DepthStencilBuffer);
	if (FAILED(result))
	{
		Shutdown();

		e.contents = _T("depth buffer 생성 실패");
		e.errorCode = result;
		throw e;
	}

	// depth stencil state 설정(depth buffer를 활성화한 depth stencil state) //
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

	// depth stencil state 생성(depth buffer를 활성화한 depth stencil state) //
	result = m_Device->CreateDepthStencilState(&DepthStencilDesc, &m_DepthStencilState);
	if (FAILED(result))
	{
		Shutdown();

		e.contents = _T("depth stencil state 생성(depth buffer를 활성화한 depth stencil state) 실패");
		e.errorCode = result;
		throw e;
	}

	// depth stencil state 설정(depth buffer를 비활성화한 depth stencil state) //
	DepthStencilDesc.DepthEnable = false;

	// depth stencil state 생성(depth buffer를 비활성화한 depth stencil state) //
	result = m_Device->CreateDepthStencilState(&DepthStencilDesc, &m_DepthDisabledStencilState);
	if (FAILED(result))
	{
		Shutdown();

		e.contents = _T("depth stencil state 생성(depth buffer를 비활성화한 depth stencil state) 실패");
		e.errorCode = result;
		throw e;
	}

	// depth stencil state(depth buffer를 활성화한 depth stencil state)를 device context에 붙이기 //
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 1);

	// depth stencil view 설정 //
	DepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDesc.Texture2D.MipSlice = 0;

	// depth stencil view 생성 // 
	result = m_Device->CreateDepthStencilView(m_DepthStencilBuffer, &DepthStencilViewDesc, &m_DepthStencilView);
	if (FAILED(result))
	{
		Shutdown();

		e.contents = _T("depth stencil view 생성 실패");
		e.errorCode = result;
		throw e;
	}

	// render target view와 depth stencil view를 출력 렌더링 파이프라인에 바인드 //
	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

	return result;
}

HRESULT D3DClass::SetRasterizer()
{
	ErrorContent e;
	HRESULT result = S_OK;
	D3D11_RASTERIZER_DESC RasterizerDesc;			// rasterizer 설정 정보

	// 에러 메세지 초기화 //
	e.title = _T("D3DClass SetRasterizer()");

	// Rasterizer 정보 입력 //
	memset(&RasterizerDesc, 0, sizeof(D3D11_RASTERIZER_DESC));
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

	// Rasterizer state 생성 //
	result = m_Device->CreateRasterizerState(&RasterizerDesc, &m_RasterizerState);
	if (FAILED(result))
	{
		e.contents = _T("Rasterizer state 생성 실패");
		e.errorCode = result;
		throw e;
	}

	// Device context에서 Rasterizer state를 설정 //
	m_DeviceContext->RSSetState(m_RasterizerState);

	return result;
}

void D3DClass::SetViewport(const int& ScreenWidth, const int& ScreenHeight)
{
	D3D11_VIEWPORT Viewport;				// view port 설정 정보

	// Viewport 정보 설정 //
	Viewport.Width = (float)ScreenWidth;
	Viewport.Height = (float)ScreenHeight;
	Viewport.MinDepth = 0.f;
	Viewport.MaxDepth = 1.f;
	Viewport.TopLeftX = 0.f;
	Viewport.TopLeftY = 0.f;

	// Viewport 생성 //
	m_DeviceContext->RSSetViewports(1, &Viewport);
}

void D3DClass::SetMatrix(const int& ScreenWidth, const int& ScreenHeight, const float& ScreenDepth, const float& ScreenNear)
{
	// Projection matrix의 시야각(Field of view), 화면 비율(aspect) 설정 //
	float FieldOfView = DirectX::XM_PI / 4.f;
	float ScreenAspect = (float)ScreenWidth / (float)ScreenHeight;

	// Projection matrix 생성 //
	m_ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(FieldOfView, ScreenAspect, ScreenNear, ScreenDepth);

	// World matrix을 항등 행렬로 생성 //
	m_WorlMatrix = DirectX::XMMatrixIdentity();

	// Ortho matrix(직교 투영 행렬) 생성 //
	m_OrthoMatrix = DirectX::XMMatrixOrthographicLH((float)ScreenWidth, (float)ScreenHeight, ScreenNear, ScreenDepth);
}

HRESULT D3DClass::SetAlphaBlendState()
{
	ErrorContent e;
	HRESULT result = S_OK;
	D3D11_BLEND_DESC AlphaBlendStateDesc;			// alpha blend state 설정 정보

	// 에러 메세지 초기화 //
	e.title = _T("D3DClass SetAlphaBlendState()");

	// alpha blend state 정보 입력 //
	memset(&AlphaBlendStateDesc, 0, sizeof(AlphaBlendStateDesc));
	AlphaBlendStateDesc.RenderTarget[0].BlendEnable = TRUE;
	AlphaBlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	AlphaBlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	AlphaBlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	AlphaBlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	AlphaBlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	AlphaBlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	AlphaBlendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// alpha blend state 생성(blend 활성화) //
	result = m_Device->CreateBlendState(&AlphaBlendStateDesc, &m_AlphaEnableBlendingState);
	if (FAILED(result))
	{
		Shutdown();

		e.contents = _T("alpha blend state 생성(blend 활성화) 실패");
		e.errorCode = result;
		throw e;
	}

	// alpha blend state 생성(blend 비활성화) //
	AlphaBlendStateDesc.RenderTarget[0].BlendEnable = FALSE;
	result = m_Device->CreateBlendState(&AlphaBlendStateDesc, &m_AlphaDisableBlendingState);
	if (FAILED(result))
	{
		Shutdown();

		e.contents = _T("alpha blend state 생성(blend 비활성화) 실패");
		e.errorCode = result;
		throw e;
	}

	return result;
}

HRESULT D3DClass::GetVideoCardDescription(IDXGIAdapter* const& Adapter)
{
	ErrorContent e;
	HRESULT result = S_OK;
	DXGI_ADAPTER_DESC AdapterDesc;			// 그래픽카드 정보
	size_t StringLength = 0;				// 그래픽카드 정보의 길이

	// 에러 메세지 초기화 //
	e.title = _T("D3DClass GetVideoCardDescription()");

	// 그래픽카드의 정보 가져오기 //
	result = Adapter->GetDesc(&AdapterDesc);
	if (FAILED(result))
	{
		Shutdown();

		e.contents = _T("그래픽카드의 정보 가져오기 실패");
		e.errorCode = result;
		throw e;
	}

	// 그래픽카드 메모리를 MB 단위로 저장 //
	m_VideoCardMemory = (int)(AdapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// 그래픽카드 정보 복사 //
	StringLength = 0;
	if (wcstombs_s(&StringLength, m_VideoCardDescription, 128, AdapterDesc.Description, 128))
	{
		Shutdown();

		e.contents = _T("그래픽카드 정보 복사 실패");
		e.errorCode = E_FAIL;
		throw e;
	}

	return result;
}
