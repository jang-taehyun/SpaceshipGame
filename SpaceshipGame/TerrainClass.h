#pragma once

namespace Graphic
{
	namespace Terrain
	{
		class QuadTreeClass;
		class SkyDomeClass;
	}
}

namespace Graphic
{
	namespace Terrain
	{
		class TerrainClass
		{
		public:
			TerrainClass();
			~TerrainClass();

			Shader::ID GetShaderID() const { return m_TerrainShaderID; }

			void Load();
			void Render(ID3D11DeviceContext* DeviceContext, D3DClass* d3d, Shader::TerrainShaderClass* TerrainShader, Object::CameraClass* Camera, DirectX::XMFLOAT4X4 world, DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 projection);

		private:
			static bool IsInitialize;

			std::unique_ptr<QuadTreeClass> m_QuadTree = nullptr;
			std::unique_ptr<SkyDomeClass> m_SkyDome = nullptr;
			Shader::ID m_TerrainShaderID = Shader::ID::NONE;
		};
	}
}