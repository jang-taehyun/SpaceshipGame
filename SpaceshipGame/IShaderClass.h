#pragma once

namespace Graphic
{
	namespace Texture { class TextureClass; }
}

namespace Graphic
{
	namespace Shader
	{
		class IShaderClass
		{
		public:
			virtual ~IShaderClass() = default;

			// shader 객체 초기화 함수 //
			virtual HRESULT Initialize(ID3D11Device* Device, const Loader::ShaderFileInfo& info, const std::vector<std::string>& VertexDataSemantics, bool IsTerrain = false) = 0;

			// vertex shader, pixel shader, input layout 바인딩하는 함수
			virtual void BeginRender(ID3D11DeviceContext* DeviceContext) = 0;

			// 렌더링
			virtual void Render(ID3D11DeviceContext* DeviceContext, int IndexCount, int InstanceCount, const std::vector<std::unique_ptr<Texture::TextureClass>>& Material) = 0;


			// 초기화 관련 함수들 //
		private:
			// shader 컴파일 및 생성 함수
			virtual HRESULT InitializeShaderInputLayout(ID3D11Device* Device, const Loader::ShaderFileInfo& info, const std::vector<std::string>& VertexDataSemantics, bool IsTerrain) = 0;

			// input layout 생성 함수
			virtual HRESULT CreateInputLayout(ID3D11Device* Device, ID3D10Blob* VertexShaderBuffer, const std::vector<std::string>& VertexDataSemantics, bool IsTerrain) = 0;

			// texture sampler state 생성 함수
			virtual HRESULT CreateTextureSamplerState(ID3D11Device* Device) = 0;

			// shader에서 사용하는 buffer들을 생성하는 함수
			virtual HRESULT CreateBuffers(ID3D11Device* Device) = 0;

			// shader 디버깅 함수 //
			virtual void OutputShaderErrorMessage(ID3D10Blob* ErrorMessage, const std::wstring& ShaderFileName) = 0;

		protected:
			// Constant buffer 생성 함수
			virtual HRESULT CreateConstantBuffer(ID3D11Device* Device, ID3D11Buffer** Buffer, UINT BufferSize) = 0;


			// 렌더링 관련 함수들 //
		private:
			virtual void BindShaderAndInputLayout(ID3D11DeviceContext* DeviceContext) = 0;
			virtual void SetShaderBuffers(ID3D11DeviceContext* DeviceContext) = 0;
		};
	}
}