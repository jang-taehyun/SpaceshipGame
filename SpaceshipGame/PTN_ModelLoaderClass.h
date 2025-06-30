#pragma once

#include "ModelLoaderClass.h"

namespace Graphic
{
	namespace Loader
	{
		// PTN vertex type //
		// struct PTN_VertexType
		// {
		// 	DirectX::XMFLOAT3 position;
		// 	DirectX::XMFLOAT2 texture;
		// 	DirectX::XMFLOAT3 normal;
		// };

		class PTN_ModelLoaderClass : public ModelLoaderClass<Graphic::Model::PTN_VertexType>
		{
		public:
			explicit PTN_ModelLoaderClass(Model::ID ModelID);
			virtual ~PTN_ModelLoaderClass() = default;

		private:
			virtual std::vector<Graphic::Model::PTN_VertexType> LoadVertexData(aiMesh* mesh) override;
			virtual HRESULT LoadMaterial(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext) override;
		};
	}
}
