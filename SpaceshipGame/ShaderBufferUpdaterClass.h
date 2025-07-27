#pragma once

#include <functional>

namespace Graphic
{
	namespace Shader { class IShaderClass; }
}

namespace Graphic
{
	namespace Shader
	{
#ifdef _DEBUG
		void UpdateCubeShaderBuffer(ID3D11DeviceContext* DeviceContext, const BuffersData& data, IShaderClass* shader);
#endif // _DEBUG

		void UpdateDefaultSpaceshipShaderBuffer(ID3D11DeviceContext* DeviceContext, const BuffersData& data, IShaderClass* shader);

		class ShaderBufferUpdaterClass
		{
		public:
			ShaderBufferUpdaterClass();
			~ShaderBufferUpdaterClass();

			void Update(ID3D11DeviceContext* DeviceContext, const BuffersData& data, ID ShaderID, IShaderClass* shader) const;

		private:
			static bool IsInitialize;
			std::map<ID, std::function<void(ID3D11DeviceContext*, const BuffersData&, IShaderClass*)>> m_UpdaterList;
		};
	}
}