#pragma once

#include "TextureClass.h"

namespace Graphic
{
	namespace Shader { class TerrainShaderClass; }
}
namespace Object { class CameraClass; }

namespace Graphic
{
	namespace Terrain
	{
		class QuadTreeClass
		{
		private:
			static const int MAX_TRAINGLES = 10000;

			struct QuadTree
			{
				int width = 0, height = 0;
				int sy = 0, sx = 0;
				int indexCount = 0;
				float max_x = 0.f, max_y = 0.f, max_z = 0.f;
				float min_x = 0.f, min_y = 0.f, min_z = 0.f;

				Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer = nullptr;
				Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer = nullptr;

				std::unique_ptr<QuadTree> nodes[4] = { nullptr, };
			};

		public:
			QuadTreeClass(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const char* HeightMap, const WCHAR* TextureFilename);
			virtual ~QuadTreeClass();

			void Render(ID3D11DeviceContext* DeviceContext, Shader::TerrainShaderClass* Shader, Object::CameraClass* Camera);

			ID3D11ShaderResourceView* GetTexture() const { return m_Texture->GetTexture(); }

		private:
			void Initialize(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const char* HeightMap, const WCHAR* TextureFilename);
			void Shutdown();

		private:
			bool CreateVertexData(ID3D11Device* Device, const char* HeightMap);
			void CreateQuadTree(ID3D11Device* Device, int sy, int sx, int current_height, int current_width, QuadTree* cur);

			void ReleaseQuadTree(QuadTree* node);

			void RenderBuffers(ID3D11DeviceContext* DeviceContext, QuadTree* node, Shader::TerrainShaderClass* Shader, Object::CameraClass* Camera);

		private:
			bool LoadHeightMap(const char* HeightMap);
			void NormalizeHeightMap();
			bool CalculateNormals();
			void CalculateTextureCoordinates();

			bool InitializeBuffers(ID3D11Device* Device, QuadTree* node);

		private:
			int m_TerrainWidth = 0;
			int m_TerrainHeight = 0;

			std::unique_ptr<QuadTree> m_QuadTree = nullptr;
			std::unique_ptr<Texture::TextureClass> m_Texture = nullptr;

			std::unique_ptr<Model::PTN_VertexType[]> m_Vertices = nullptr;
		};
	}
}

