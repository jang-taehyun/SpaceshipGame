#pragma once

#include <functional>

namespace Graphic
{
	namespace Shader { class IShaderClass; }
}

namespace Graphic
{
	namespace Loader
	{
#ifdef _DEBUG
		std::unique_ptr<Shader::IShaderClass> CreateCubeShader(HWND hwnd, ID3D11Device* Device, Shader::ID id);
#endif
		std::unique_ptr<Shader::IShaderClass> CreateDefaultSpaceshipShader(HWND hwnd, ID3D11Device* Device, Shader::ID id);
		std::unique_ptr<Shader::IShaderClass> CreateDefaultTerrainShader(HWND hwnd, ID3D11Device* Device, Shader::ID id);

		class ShaderFactoryClass
		{
		private:
			const UINT m_ShaderIDCount = 4;

		public:
			ShaderFactoryClass();
			~ShaderFactoryClass();

			UINT GetShaderIDCount() const { return m_ShaderIDCount; }

			std::unique_ptr<Shader::IShaderClass> Load(HWND hwnd, ID3D11Device* Device, Shader::ID id) const;

		private:
			static bool IsInitialize;

			std::map<Shader::ID, std::function<std::unique_ptr<Shader::IShaderClass>(HWND, ID3D11Device*, Shader::ID)>> m_Creator;
		};
	}
}
