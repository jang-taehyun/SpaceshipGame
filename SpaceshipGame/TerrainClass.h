#pragma once

namespace Graphic
{
	namespace Terrain
	{
		class QuadTreeClass;
		class SkyDomeClass;
	}
	namespace Shader { class IShaderClass; }
}

namespace Object { class IObjectClass; }

namespace Graphic
{
	namespace Terrain
	{
		class TerrainClass
		{
		private:
			std::map<TerrainID, const char*> m_HeightMapList =
			{
				{ TerrainID::DEFAULT, "./resource/heightmap01.bmp" },
			};

			std::map<TerrainID, const WCHAR*> m_TerrainTextureList =
			{
				{ TerrainID::DEFAULT, _T("./resource/dirt01.dds") },
			};

			std::map<SkyDomeID, const WCHAR*> m_SkyDomeTextureList =
			{

			};

			std::map<TerrainID, Shader::ID> m_TerrainShaderIDList =
			{
				{ TerrainID::DEFAULT, Shader::ID::TERRAIN },
			};

		public:
			TerrainClass();
			~TerrainClass();

			Shader::ID GetShaderID() const { return m_TerrainShaderID; }

			void Load(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, TerrainID terrain_id, SkyDomeID skydome_id);
			void Render(ID3D11DeviceContext* DeviceContext, D3DClass* d3d, Shader::IShaderClass* TerrainShader, Object::IObjectClass* Camera, DirectX::XMFLOAT4X4 projection);

		private:
			static bool IsInitialize;

			std::unique_ptr<QuadTreeClass> m_QuadTree = nullptr;
			std::unique_ptr<SkyDomeClass> m_SkyDome = nullptr;
			Shader::ID m_TerrainShaderID = Shader::ID::NONE;
		};
	}
}