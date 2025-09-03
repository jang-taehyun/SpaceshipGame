#pragma once

#include "ModelLoaderClass.hpp"

namespace Graphic
{
	namespace Loader
	{
		class P_ModelLoaderClass : public ModelLoaderClass<Graphic::Model::P_VertexType>
		{
		public:
			P_ModelLoaderClass() = default;
			virtual ~P_ModelLoaderClass() = default;

		private:
			virtual std::vector<Graphic::Model::P_VertexType> LoadVertexData(aiMesh* mesh) override;
			virtual void LoadMaterial(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const aiScene* scene, const std::wstring& AdditionalPath) override;
		};
	}
}
