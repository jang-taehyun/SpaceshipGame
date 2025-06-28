#pragma once

namespace Graphic { class LightClass; }
namespace Object { class IObjectClass; }

namespace Graphic
{
	namespace Shader
	{
		// TODO: ShaderClass를 가상 팩토리 패턴으로 생성하게 만들기
		class ShaderClass
		{
		protected:
			struct MatrixBufferType
			{
				DirectX::XMMATRIX World;
				DirectX::XMMATRIX View;
				DirectX::XMMATRIX Projection;
			};

			struct LightBufferType
			{
				DirectX::XMFLOAT4 AmbientColor;
				DirectX::XMFLOAT4 DiffuseColor;
				DirectX::XMFLOAT3 LightDirection;
				float padding1 = 0.f;
				DirectX::XMFLOAT4 SpecularColor;
				float SpecularPower;
				float padding2[3] = { 0.f, };
			};

			struct CameraBufferType
			{
				DirectX::XMFLOAT3 CameraPosition;
				float padding;
			};

		public:
			// 생성자, 소멸자 //
			ShaderClass(HWND hwnd, ID3D11Device* Device, const std::vector<std::string>& VertexDataSemantics, ID ShaderID);
			virtual ~ShaderClass() = default;

			// shader 렌더링 준비 함수 //
			virtual void BeginRender(ID3D11DeviceContext* DeviceContext, const TransformMatrixData& transform, const LightClass* light, const Object::IObjectClass* camera);

			// shader 렌더링 함수 //
			virtual HRESULT Render(ID3D11DeviceContext* DeviceContext, int IndexCount, const std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& Material);


			// 초기화 관련 함수들 //
		private:
			// shader 객체 초기화 함수 //
			HRESULT Initialize(HWND hwnd, ID3D11Device* Device, const std::vector<std::string>& VertexDataSemantics);

			// shader 컴파일 및 생성 함수
			HRESULT InitializeShader(HWND hwnd, ID3D11Device* Device, const std::vector<std::string>& VertexDataSemantics);

			// input layout 생성 함수
			HRESULT CreateInputLayout(ID3D11Device* Device, ID3D10Blob* VertexShaderBuffer, const std::vector<std::string>& VertexDataSemantics);

			// texture sampler state 생성 함수
			HRESULT CreateTextureSamplerState(ID3D11Device* Device);

			// Constant buffer 생성 함수
			HRESULT CreateConstantBuffer(ID3D11Device* Device, ID3D11Buffer** Buffer, UINT BufferSize);

			// shader 디버깅 함수 //
			void OutputShaderErrorMessage(HWND hwnd, ID3D10Blob* ErrorMessage, const std::wstring& ShaderFileName);


			// 렌더링 관련 함수들 //
		private:
			// vertex shader, pixel shader, input layout 바인딩하는 함수 //
			void BindShaderAndInputLayout(ID3D11DeviceContext* DeviceContext);

			// shader 내부에 들어갈 데이터들 업데이트하는 함수 //
			void SetShaderBuffers(ID3D11DeviceContext* DeviceContext, const TransformMatrixData& transform, const LightClass* light, const Object::IObjectClass* camera);

			// Matrix buffer를 업데이트 하는 함수 //
			HRESULT UpdateMatrixBuffer(ID3D11DeviceContext* DeviceContext, UINT slot, const TransformMatrixData& transform);

			// Light buffer를 업데이트 하는 함수 //
			HRESULT UpdateLightBuffer(ID3D11DeviceContext* DeviceContext, UINT slot, const LightClass* light);

			// camera buffer를 업데이트 하는 함수 //
			HRESULT UpdateCameraBuffer(ID3D11DeviceContext* DeviceContext, UINT slot, const Object::IObjectClass* camera);

		private:
			ID m_ShaderID = ID::DEFAULT;

			Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader = nullptr;
			Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader = nullptr;
			Microsoft::WRL::ComPtr<ID3D11InputLayout> m_Layout = nullptr;
			Microsoft::WRL::ComPtr<ID3D11Buffer> m_MatrixBuffer = nullptr;
			Microsoft::WRL::ComPtr<ID3D11Buffer> m_LightBuffer = nullptr;
			Microsoft::WRL::ComPtr<ID3D11Buffer> m_CameraBuffer = nullptr;
			Microsoft::WRL::ComPtr<ID3D11SamplerState> m_SampleState = nullptr;

		public:
			ShaderClass(const ShaderClass& other) = delete;
			ShaderClass(ShaderClass&& other) = delete;
			ShaderClass& operator=(const ShaderClass& other) = delete;
			ShaderClass& operator=(ShaderClass&& other) = delete;
		};
	}
}

