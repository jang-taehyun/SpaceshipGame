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

			virtual void Load(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext) = 0;

			virtual inline ULONG GetMeshCount() const = 0;
			virtual inline DirectX::BoundingOrientedBox GetModelOBB() const = 0;
			virtual inline std::vector<std::vector<ULONG>> MoveIndicesDatas() = 0;
			virtual inline std::vector<std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>> MoveMaterialsDatas() = 0;
		};
	}
}

