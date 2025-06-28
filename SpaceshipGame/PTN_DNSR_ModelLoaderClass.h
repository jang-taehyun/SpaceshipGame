#pragma once

#include "ModelLoaderClass.h"

namespace Graphic
{
	namespace Loader
	{
		// PTN vertex type //
		struct PTN_VertexType
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT2 texture;
			DirectX::XMFLOAT3 normal;
		};

		class PTN_ModelLoaderClass : public ModelLoaderClass<PTN_VertexType>
		{
		public:
			explicit PTN_ModelLoaderClass(const std::wstring& filename);
			virtual ~PTN_ModelLoaderClass() = default;

			virtual HRESULT Load(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext) override;

		private:
			virtual HRESULT LoadVertexData();
			virtual HRESULT LoadMaterialData(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext);

			virtual HRESULT LoadTexture(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& MaterialList, const aiString& TexturePath);
		};
	}
}


