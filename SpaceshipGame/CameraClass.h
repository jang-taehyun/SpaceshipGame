#pragma once

/**
* CameraClass °³¿ä
*/

#include <DirectXCollision.h>
#include "ObjectClass.h"

namespace Object
{
	class CameraClass : public ObjectClass
	{
	public:
		CameraClass() = default;
		virtual ~CameraClass() = default;

		void UpdateFrustum(DirectX::XMFLOAT4X4 projection);
		DirectX::XMFLOAT4X4 Render() const;

		bool IsRender(DirectX::BoundingOrientedBox ModelOBB, DirectX::XMFLOAT4X4 ModelWorldMatrix) const;

		virtual inline std::unique_ptr<IObjectClass> Clone() const override { return std::make_unique<CameraClass>(*this); }

	private:
		DirectX::BoundingFrustum m_Frustum;
	};
}