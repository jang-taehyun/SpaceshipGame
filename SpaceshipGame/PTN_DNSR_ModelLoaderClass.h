#pragma once

#include "ModelLoaderClass.cpp"

namespace Graphic
{
	namespace Loader
	{
		class PTN_DNSR_ModelLoaderClass : public ModelLoaderClass<Graphic::Model::PTN_VertexType>
		{
		public:
			PTN_DNSR_ModelLoaderClass() = default;
			virtual ~PTN_DNSR_ModelLoaderClass() = default;
			
		private:
			virtual void LoadVertexData(aiMesh* mesh, std::vector<Model::PTN_VertexType>& vertices) override;
			virtual void LoadMaterial(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const aiScene* scene, const std::wstring& AdditionalPath) override;
		};
	}
}


