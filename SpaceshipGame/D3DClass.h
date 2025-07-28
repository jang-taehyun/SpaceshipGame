#pragma once

namespace Graphic
{
	class D3DClass
	{
	public:
		D3DClass(HWND hwnd, int ScreenWidth, int ScreenHeight);
		virtual ~D3DClass();

		// back buffer를 지우는 함수 //
		void BeginScene(DirectX::XMFLOAT4 color);

		// 화면에 back buffer의 내용을 표시하는 함수
		void EndScene() const;

		inline ID3D11Device* GetDevice() const { return m_Device.Get(); }
		inline ID3D11DeviceContext* GetDeviceContext() const { return m_DeviceContext.Get(); }

		inline DirectX::XMFLOAT4X4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline DirectX::XMFLOAT4X4 GetOrthoMatrix() const { return m_OrthoMatrix; }

		void TurnDepthBufferOn() const;
		void TurnDepthBufferOff() const;

		void TurnOnAlphaBlending() const;
		void TurnOffAlphaBlending() const;

	private:
		// D3D 객체 초기화 함수 //
		HRESULT Initialize(HWND hwnd, int ScreenWidth, int ScreenHeight);

		// D3D 객체 내부 리소스 정리 함수 //
		void Shutdown();

		// 적절한 디스플레이 모드를 찾는 함수
		HRESULT GetRefreshRate(int ScreenWidth, int ScreenHeight, int& Numerator, int& Denominator);

		// Swap chain 설정 및 Swap chain, Device, Device context 생성하는 함수
		HRESULT CreateSwapChainDeviceDeviceContext(HWND hwnd, int ScreenWidth, int ScreenHeight, int Numerator, int Denominator);

		// Render target view 생성 및 설정하는 함수
		HRESULT SetAndCreateRenderTargetView();

		// Depth, Stencil 설정 함수
		HRESULT SetDepthAndStencil(int ScreenWidth, int ScreenHeight);

		// Rasterizer 설정 함수
		HRESULT SetRasterizer();

		// 렌더링을 위한 Viewport 설정 함수
		void SetViewport(int ScreenWidth, int ScreenHeight);

		// Matrix 설정 함수
		void SetMatrix(int ScreenWidth, int ScreenHeight);

		// alpha blending state 설정 함수
		HRESULT SetAlphaBlendState();

	private:
		static bool IsInitialize;

		// swap chain
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain = nullptr;

		// device, device context, render target view
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext = nullptr;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView = nullptr;

		// depth stencil
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_DepthStencilBuffer = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DepthStencilState = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DepthDisabledStencilState = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DepthStencilView = nullptr;

		// rasterizer
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_RasterizerState = nullptr;

		// matrix
		DirectX::XMFLOAT4X4 m_ProjectionMatrix;
		DirectX::XMFLOAT4X4 m_OrthoMatrix;

		// alpha blending
		Microsoft::WRL::ComPtr<ID3D11BlendState> m_AlphaEnableBlendingState = nullptr;
		Microsoft::WRL::ComPtr<ID3D11BlendState> m_AlphaDisableBlendingState = nullptr;

	public:
		D3DClass(const D3DClass& other) = delete;
		D3DClass(D3DClass&& other) = delete;
		D3DClass& operator=(const D3DClass& other) = delete;
		D3DClass& operator=(D3DClass&& other) = delete;
	};
}