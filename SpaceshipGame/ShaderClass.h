#pragma once

#include "IShaderClass.h"

namespace Graphic
{
	namespace Shader
	{
		template<typename ShaderBuffers>
		class ShaderClass : public IShaderClass
		{
		public:
			explicit ShaderClass(ID ShaderID);
			ShaderClass(const ShaderClass& other);
			ShaderClass(ShaderClass&& other) noexcept;
			virtual ~ShaderClass() = default;

			ShaderClass<ShaderBuffers>& operator=(const ShaderClass& other);
			ShaderClass<ShaderBuffers>& operator=(ShaderClass&& other) noexcept;

			// shader 객체 초기화 함수 //
			virtual HRESULT Initialize(HWND hwnd, ID3D11Device* Device, const Loader::ShaderFileInfo& info, const std::vector<std::string>& VertexDataSemantics, bool IsTerrain = false) override;


			// 렌더링 함수들 //

			// shader 내부에 들어갈 데이터들 업데이트하는 함수 //
			virtual void UpdateShaderBuffers(ID3D11DeviceContext* DeviceContext, const ShaderBuffers& ShaderBufferDatas) = 0;

			// vertex shader, pixel shader, input layout 바인딩하는 함수
			virtual void BeginRender(ID3D11DeviceContext* DeviceContext) override;

			// shader 렌더링 함수
			virtual void Render(ID3D11DeviceContext* DeviceContext, int IndexCount, int InstanceCount, const std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& Material) override;


			// 초기화 관련 함수들 //
		private:
			// shader 컴파일 및 생성 함수
			virtual HRESULT InitializeShaderInputLayout(HWND hwnd, ID3D11Device* Device, const Loader::ShaderFileInfo& info, const std::vector<std::string>& VertexDataSemantics, bool IsTerrain) override;

			// input layout 생성 함수
			virtual HRESULT CreateInputLayout(ID3D11Device* Device, ID3D10Blob* VertexShaderBuffer, const std::vector<std::string>& VertexDataSemantics, bool IsTerrain) override;

			// texture sampler state 생성 함수
			virtual HRESULT CreateTextureSamplerState(ID3D11Device* Device) override;

			// shader에서 사용하는 buffer들을 생성하는 함수
			virtual HRESULT CreateBuffers(ID3D11Device* Device) override = 0;

			// shader 디버깅 함수 //
			virtual void OutputShaderErrorMessage(HWND hwnd, ID3D10Blob* ErrorMessage, const std::wstring& ShaderFileName) override;

		protected:
			// Constant buffer 생성 함수
			virtual HRESULT CreateConstantBuffer(ID3D11Device* Device, ID3D11Buffer** Buffer, UINT BufferSize) override;

			// 렌더링 관련 함수들 //
		private:
			virtual void BindShaderAndInputLayout(ID3D11DeviceContext* DeviceContext) override;
			virtual void SetShaderBuffers(ID3D11DeviceContext* DeviceContext) override = 0;

		private:
			ID m_ShaderID = ID::DEFAULT;

			Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader = nullptr;
			Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader = nullptr;
			Microsoft::WRL::ComPtr<ID3D11InputLayout> m_Layout = nullptr;
			Microsoft::WRL::ComPtr<ID3D11SamplerState> m_SampleState = nullptr;
		};
	}
}