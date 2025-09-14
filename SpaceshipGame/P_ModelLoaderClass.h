#pragma once

#include "ModelLoaderClass.cpp"

namespace Graphic
{
	namespace Loader
	{
		class P_ModelLoaderClass : public ModelLoaderClass<Model::P_VertexType>
		{
		public:
			P_ModelLoaderClass() = default;
			virtual ~P_ModelLoaderClass() = default;

		private:
			virtual void LoadVertexData(aiMesh* mesh, std::vector<Model::P_VertexType>& vertices) override;
			virtual void LoadMaterial(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const aiScene* scene, const std::wstring& AdditionalPath) override;
		};
	}
}
