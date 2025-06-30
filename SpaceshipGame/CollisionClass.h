#pragma once

/**
* CollisionClass °³¿ä
*/

#include <DirectXCollision.h>
#include "GameObjectClass.h"

namespace Object
{
	class CollisionClass : public GameObjectClass
	{
	public:
		CollisionClass(const AffineInfo& affine, float range = 0.f);
		virtual ~CollisionClass() = default;

		virtual DirectX::ContainmentType GetCollideState(const AffineInfo& affine);
		virtual DirectX::ContainmentType GetCollideState(DirectX::XMFLOAT4 position, DirectX::XMFLOAT4 forward);

		virtual inline DirectX::XMFLOAT4 GetColor() const { return m_Color; }
		virtual inline void SetColor(DirectX::XMFLOAT4 color) { m_Color = color; }

		virtual inline std::unique_ptr<IObjectClass> Clone() const override;

	private:
		DirectX::XMFLOAT4 m_Color = DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f);
		float m_Range = 10.f;
	};
}