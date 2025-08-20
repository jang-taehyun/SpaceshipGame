#pragma once

#include <DirectXCollision.h>

namespace Graphic
{
	namespace Loader
	{
		class IModelLoaderClass
		{
		public:
			virtual ~IModelLoaderClass() = default;

			virtual void Load(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const std::string& ModelFilename, const std::wstring& AdditionalPath) = 0;

			virtual ULONG GetMeshCount() const = 0;
			virtual DirectX::BoundingOrientedBox GetModelOBB() const = 0;
			virtual std::vector<std::vector<ULONG>> MoveIndicesDatas() = 0;
			virtual std::vector<std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>> MoveMaterialsDatas() = 0;
		};
	}
}

