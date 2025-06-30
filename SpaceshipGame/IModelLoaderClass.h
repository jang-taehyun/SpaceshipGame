#pragma once

namespace Graphic
{
	namespace Loader
	{
		class IModelLoaderClass
		{
		public:
			virtual ~IModelLoaderClass() = default;

			virtual HRESULT Load(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext) = 0;
		};
	}
}

