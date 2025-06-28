#pragma once

namespace Graphic
{
	namespace Model
	{
		class IModelClass
		{
		public:
			virtual ~IModelClass() = default;
			virtual void RenderMesh(ID3D11DeviceContext* DeviceContext, int MeshIdx) = 0;
			virtual inline ULONG GetIndexCount(int idx) const = 0;

		private:
			virtual HRESULT Initialize(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext) = 0;
		};
	}
}