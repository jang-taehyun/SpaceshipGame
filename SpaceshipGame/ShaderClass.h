#pragma once

#include "IShaderClass.h"

namespace Graphic { class LightClass; }
namespace Object { class IObjectClass; }

namespace Graphic
{
	namespace Shader
	{
		template<typename ShaderBuffers>
		class ShaderClass : public IShaderClass
		{
		public:
			explicit ShaderClass(ID ShaderID);
			virtual ~ShaderClass() = default;

			// shader 객체 초기화 함수 //
			virtual HRESULT Initialize(HWND hwnd, ID3D11Device* Device, const std::vector<std::string>& VertexDataSemantics) override;

			// 렌더링 함수들 //
			// 
			// 렌더링 준비 함수
			void BeginRender(ID3D11DeviceContext* DeviceContext, const ShaderBuffers& ShaderBufferDatas);

			// shader 렌더링 함수
			virtual HRESULT Render(ID3D11DeviceContext* DeviceContext, int IndexCount, const std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& Material) override;


			// 초기화 관련 함수들 //
		private:
			// shader 컴파일 및 생성 함수
			HRESULT InitializeShader(HWND hwnd, ID3D11Device* Device, const std::vector<std::string>& VertexDataSemantics) override;

			// input layout 생성 함수
			HRESULT CreateInputLayout(ID3D11Device* Device, ID3D10Blob* VertexShaderBuffer, const std::vector<std::string>& VertexDataSemantics) override;

			// texture sampler state 생성 함수
			HRESULT CreateTextureSamplerState(ID3D11Device* Device) override;

			// shader에서 사용하는 buffer들을 생성하는 함수
			virtual HRESULT CreateBuffers(ID3D11Device* Device) override = 0;

			// shader 디버깅 함수 //
			void OutputShaderErrorMessage(HWND hwnd, ID3D10Blob* ErrorMessage, const std::wstring& ShaderFileName) override;

		protected:
			// Constant buffer 생성 함수
			HRESULT CreateConstantBuffer(ID3D11Device* Device, ID3D11Buffer** Buffer, UINT BufferSize) override;


			// 렌더링 관련 함수들 //
		private:
			// vertex shader, pixel shader, input layout 바인딩하는 함수
			virtual void BindShaderAndInputLayout(ID3D11DeviceContext* DeviceContext) override;

			// shader 내부에 들어갈 데이터들 업데이트하는 함수 //
			virtual void SetShaderBuffers(ID3D11DeviceContext* DeviceContext, const ShaderBuffers& ShaderBufferDatas) = 0;

		private:
			ID m_ShaderID = ID::DEFAULT;

			Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader = nullptr;
			Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader = nullptr;
			Microsoft::WRL::ComPtr<ID3D11InputLayout> m_Layout = nullptr;
			Microsoft::WRL::ComPtr<ID3D11SamplerState> m_SampleState = nullptr;

		public:
			ShaderClass(const ShaderClass& other) = delete;
			ShaderClass(ShaderClass&& other) = delete;
			ShaderClass& operator=(const ShaderClass& other) = delete;
			ShaderClass& operator=(ShaderClass&& other) = delete;
		};
	}
}

