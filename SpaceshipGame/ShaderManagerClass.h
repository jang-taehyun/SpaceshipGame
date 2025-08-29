#pragma once

namespace Graphic
{
	namespace Loader { class ShaderFactoryClass; }
	namespace Shader
	{
		class IShaderClass;
		class ShaderBufferUpdaterClass;
	}
}

namespace Graphic
{
	namespace Shader
	{
		class ShaderManagerClass
		{
		public:
			ShaderManagerClass();
			~ShaderManagerClass();

			IShaderClass* GetShader(ID key) const;

			void Load(ID3D11Device* Device, UINT ShaderMask);
			void UpdateBuffer(ID3D11DeviceContext* DeviceContext, const BuffersData& data);

			UINT GetShaderMask() const { return m_CurrentShaderMask; }

		private:
			static bool IsInitialize;

			std::map<ID, std::unique_ptr<IShaderClass>> m_ShaderList;
			std::unique_ptr<Loader::ShaderFactoryClass> m_Factory = nullptr;
			std::unique_ptr<ShaderBufferUpdaterClass> m_Updater = nullptr;
			UINT m_CurrentShaderMask = 0;
		};
	}
}