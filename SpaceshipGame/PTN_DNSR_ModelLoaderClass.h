#pragma once

#include "ModelLoaderClass.hpp"

namespace Graphic
{
	namespace Loader
	{
		class PTN_DNSR_ModelLoaderClass : public ModelLoaderClass<Graphic::Model::PTN_VertexType>
		{
		public:
			explicit PTN_DNSR_ModelLoaderClass(Model::ID ModelID);
			virtual ~PTN_DNSR_ModelLoaderClass() = default;
			
		private:
			virtual std::vector<Graphic::Model::PTN_VertexType> LoadVertexData(aiMesh* mesh) override;
			virtual void LoadMaterial(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const aiScene* scene) override;
		};
	}
}


