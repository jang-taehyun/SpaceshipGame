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
		};
	}
}