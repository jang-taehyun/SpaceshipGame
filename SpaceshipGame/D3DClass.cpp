#include "pch.h"
#include "D3DClass.h"

bool Graphic::D3DClass::IsInitialize = false;

Graphic::D3DClass::D3DClass(HWND hwnd, int ScreenWidth, int ScreenHeight)
{
	HRESULT result = S_OK;

	assert(!IsInitialize);

	result = Initialize(hwnd, ScreenWidth, ScreenHeight);
	IsInitialize = true;
}

Graphic::D3DClass::~D3DClass()
{
	Shutdown();
	IsInitialize = false;
}

HRESULT Graphic::D3DClass::Initialize(HWND hwnd, int ScreenWidth, int ScreenHeight)
{
	HRESULT result = S_OK;
	int Numerator = 0, Denominator = 0;

	// 1. DirectX Graphics Infrastructure(DXGI)를 통해 적절한 디스플레이 모드 찾기 및 적용
	result = GetRefreshRate(ScreenWidth, ScreenHeight, Numerator, Denominator);
	assert(SUCCEEDED(result));

	// 2. Swap chain 설정 및 Swap chain, Device, Device context 생성
	result = CreateSwapChainDeviceDeviceContext(hwnd, ScreenWidth, ScreenHeight, Numerator, Denominator);
	assert(SUCCEEDED(result));

	// 3. Render target view 생성 및 설정
	result = SetAndCreateRenderTargetView();
	assert(SUCCEEDED(result));

	// 4. Depth Stencil buffer 생성
	result = SetDepthAndStencil(ScreenWidth, ScreenHeight);
	assert(SUCCEEDED(result));

	// 5. Rasterizer 설정
	result = SetRasterizer();
	assert(SUCCEEDED(result));
	
	// 6. 렌더링을 위한 Viewport 설정
	SetViewport(ScreenWidth, ScreenHeight);
	
	// 7. Matrix 설정
	SetMatrix(ScreenWidth, ScreenHeight);

	// 8. alpha blending state 설정
	result = SetAlphaBlendState();
	assert(SUCCEEDED(result));

	return result;
}

void Graphic::D3DClass::Shutdown()
{
	// full screen 모드인 경우, 윈도우 모드로 변경
	// swap chain 해제 시에, 윈도우 모드로 설정하지 않으면 해제할 때 예외가 발생
	if (m_SwapChain)
		m_SwapChain->SetFullscreenState(false, NULL);
}

void Graphic::D3DClass::BeginScene(DirectX::XMFLOAT4 color)
{
	// back buffer를 지울 색상 설정
	float background[4] = { color.x, color.y, color.z, color.w };

	// back buffer 초기화
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), background);

	// depth buffer 초기화
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
}

void Graphic::D3DClass::EndScene() const
{
	if (System::VSYNC_ENABLED)
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

void Graphic::D3DClass::TurnDepthBufferOn() const
{
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState.Get(), 1);
}

void Graphic::D3DClass::TurnDepthBufferOff() const
{
	m_DeviceContext->OMSetDepthStencilState(m_DepthDisabledStencilState.Get(), 1);
}

void Graphic::D3DClass::TurnOnAlphaBlending() const
{
	// blend factor를 설정하고, alpha blending state 활성화 //
	float BlendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
	m_DeviceContext->OMSetBlendState(m_AlphaEnableBlendingState.Get(), BlendFactor, 0xffffffff);
}

void Graphic::D3DClass::TurnOffAlphaBlending() const
{
	// blend factor를 설정하고, alpha blending state 비활성화 //
	float BlendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
	m_DeviceContext->OMSetBlendState(m_AlphaDisableBlendingState.Get(), BlendFactor, 0xffffffff);
}

HRESULT Graphic::D3DClass::GetRefreshRate(int ScreenWidth, int ScreenHeight, int& Numerator, int& Denominator)
{
	HRESULT result = S_OK;
	Microsoft::WRL::ComPtr<IDXGIFactory> Factory = nullptr;			// DXGI factory
	Microsoft::WRL::ComPtr<IDXGIAdapter> Adapter = nullptr;			// 기본 그래픽 카드
	Microsoft::WRL::ComPtr<IDXGIOutput> AdapterOutput = nullptr;	// 기본 모니터
	UINT ModeCount = 0, CombinationCount = 0;						// format에 맞는 display mode의 개수, 디스플레이 모드에 대한 조합의 개수
	std::unique_ptr<DXGI_MODE_DESC[]> DisplayModeList = nullptr;	// display mode의 정보를 담은 배열

	// DXGI factory 생성 //
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(Factory.GetAddressOf()));
	assert(SUCCEEDED(result));

	// 기본 그래픽카드 및 기본 모니터 조회 //
	// 기본 그래픽카드
	result = Factory->EnumAdapters(0, Adapter.GetAddressOf());
	assert(SUCCEEDED(result));

	// 기본 모니터
	result = Adapter->EnumOutputs(0, AdapterOutput.GetAddressOf());
	assert(SUCCEEDED(result));

	// 모니터에서 DXGI_FORMAT_R8G8B8A8_UNORM 표시 형식에 맞는 모드 수 조회 //
	result = AdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &ModeCount, NULL);
	assert(SUCCEEDED(result));

	// 디스플레이 모드에 대한 모든 조합을 구하기 //
	CombinationCount = ModeCount;
	DisplayModeList = std::make_unique<DXGI_MODE_DESC[]>(CombinationCount);
	assert(SUCCEEDED(result));

	result = AdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &ModeCount, DisplayModeList.get());
	assert(SUCCEEDED(result));

	// 모든 조합에서 윈도우의 가로, 세로 길이에 맞는 디스플레이 모드를 찾고, FPS의 분모, 분자 값 저장 //
	for (UINT i = 0; i < CombinationCount; ++i)
	{
		if (DisplayModeList[i].Width == (unsigned int)ScreenWidth)
		{
			if (DisplayModeList[i].Height == (unsigned int)ScreenHeight)
			{
				Numerator = DisplayModeList[i].RefreshRate.Numerator;
				Denominator = DisplayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	return result;
}

HRESULT Graphic::D3DClass::CreateSwapChainDeviceDeviceContext(HWND hwnd, int ScreenWidth, int ScreenHeight, int Numerator, int Denominator)
{
	HRESULT result = S_OK;
	DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};		// swap chain 설정 정보
	D3D_FEATURE_LEVEL FeatureLevel;					// DirectX 버전 정보

	// swap chain 설정 //
	SwapChainDesc.BufferCount = 1;									// back buffer의 개수
	SwapChainDesc.BufferDesc.Width = ScreenWidth;					// back buffer의 해상도
	SwapChainDesc.BufferDesc.Height = ScreenHeight;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// back buffer의 format

	// swap chain의 refresh rate 설정 //
	if (System::VSYNC_ENABLED)
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
	if (System::FULL_SCREEN)
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
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &FeatureLevel, 1, D3D11_SDK_VERSION, &SwapChainDesc, m_SwapChain.GetAddressOf(), m_Device.GetAddressOf(), NULL, m_DeviceContext.GetAddressOf());
	assert(SUCCEEDED(result));

	return result;
}

HRESULT Graphic::D3DClass::SetAndCreateRenderTargetView()
{
	HRESULT result = S_OK;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> BackBufferPtr = nullptr;			// back buffer의 포인터

	// back buffer의 pointer 가져오기 //
	result = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(BackBufferPtr.GetAddressOf()));
	assert(SUCCEEDED(result));

	// render target view를 생성하고, render target view가 back buffer를 가르키도록 설정 //
	result = m_Device->CreateRenderTargetView(BackBufferPtr.Get(), NULL, m_RenderTargetView.GetAddressOf());
	assert(SUCCEEDED(result));

	return result;
}

HRESULT Graphic::D3DClass::SetDepthAndStencil(int ScreenWidth, int ScreenHeight)
{
	HRESULT result = S_OK;
	D3D11_TEXTURE2D_DESC DepthBufferDesc = {};					// depth buffer 설정 정보
	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc = {};				// depth stencil state 설정 정보
	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc = {};	// depth stencil view 설정 정보

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
	result = m_Device->CreateTexture2D(&DepthBufferDesc, NULL, m_DepthStencilBuffer.GetAddressOf());
	assert(SUCCEEDED(result));

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
	result = m_Device->CreateDepthStencilState(&DepthStencilDesc, m_DepthStencilState.GetAddressOf());
	assert(SUCCEEDED(result));

	// depth stencil state 설정(depth buffer를 비활성화한 depth stencil state) //
	DepthStencilDesc.DepthEnable = false;

	// depth stencil state 생성(depth buffer를 비활성화한 depth stencil state) //
	result = m_Device->CreateDepthStencilState(&DepthStencilDesc, m_DepthDisabledStencilState.GetAddressOf());
	assert(SUCCEEDED(result));

	// depth stencil state(depth buffer를 활성화한 depth stencil state)를 device context에 붙이기 //
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState.Get(), 1);

	// depth stencil view 설정 //
	DepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDesc.Texture2D.MipSlice = 0;

	// depth stencil view 생성 // 
	result = m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &DepthStencilViewDesc, m_DepthStencilView.GetAddressOf());
	assert(SUCCEEDED(result));

	// render target view와 depth stencil view를 출력 렌더링 파이프라인에 바인드 //
	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());

	return result;
}

HRESULT Graphic::D3DClass::SetRasterizer()
{
	HRESULT result = S_OK;
	D3D11_RASTERIZER_DESC RasterizerDesc = {};			// rasterizer 설정 정보

	// Rasterizer 정보 입력 //
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
	result = m_Device->CreateRasterizerState(&RasterizerDesc, m_RasterizerState.GetAddressOf());
	assert(SUCCEEDED(result));

	// Device context에서 Rasterizer state를 설정 //
	m_DeviceContext->RSSetState(m_RasterizerState.Get());

	return result;
}

void Graphic::D3DClass::SetViewport(int ScreenWidth, int ScreenHeight)
{
	D3D11_VIEWPORT Viewport = {};						// view port 설정 정보

	// Viewport 정보 설정 //
	Viewport.Width = static_cast<float>(ScreenWidth);
	Viewport.Height = static_cast<float>(ScreenHeight);
	Viewport.MinDepth = 0.f;
	Viewport.MaxDepth = 1.f;
	Viewport.TopLeftX = 0.f;
	Viewport.TopLeftY = 0.f;

	// Viewport 생성 //
	m_DeviceContext->RSSetViewports(1, &Viewport);
}

void Graphic::D3DClass::SetMatrix(int ScreenWidth, int ScreenHeight)
{
	DirectX::XMMATRIX proj, ortho;

	// Projection matrix의 시야각(Field of view), 화면 비율(aspect) 설정 //
	float FieldOfView = DirectX::XM_PI / 4.f;
	float ScreenAspect = static_cast<float>(ScreenWidth) / static_cast<float>(ScreenHeight);

	// Projection matrix 생성 //
	proj = DirectX::XMMatrixPerspectiveFovLH(FieldOfView, ScreenAspect, System::SCREEN_NEAR, System::SCREEN_DEPTH);
	DirectX::XMStoreFloat4x4(&m_ProjectionMatrix, proj);

	// Ortho matrix(직교 투영 행렬) 생성 //
	ortho = DirectX::XMMatrixOrthographicLH(static_cast<float>(ScreenWidth), static_cast<float>(ScreenHeight), System::SCREEN_NEAR, System::SCREEN_DEPTH);
	DirectX::XMStoreFloat4x4(&m_OrthoMatrix, ortho);
}

HRESULT Graphic::D3DClass::SetAlphaBlendState()
{
	HRESULT result = S_OK;
	D3D11_BLEND_DESC AlphaBlendStateDesc = {};			// alpha blend state 설정 정보

	// alpha blend state 정보 입력 //
	AlphaBlendStateDesc.RenderTarget[0].BlendEnable = TRUE;
	AlphaBlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	AlphaBlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	AlphaBlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	AlphaBlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	AlphaBlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	AlphaBlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	AlphaBlendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// alpha blend state 생성(blend 활성화) //
	result = m_Device->CreateBlendState(&AlphaBlendStateDesc, m_AlphaEnableBlendingState.GetAddressOf());
	assert(SUCCEEDED(result));

	// alpha blend state 생성(blend 비활성화) //
	AlphaBlendStateDesc.RenderTarget[0].BlendEnable = FALSE;
	result = m_Device->CreateBlendState(&AlphaBlendStateDesc, m_AlphaDisableBlendingState.GetAddressOf());
	assert(SUCCEEDED(result));

	return result;
}
