#pragma once

/**
* CollisionClass °³¿ä
*/

#include "ICollisionClass.h"

namespace Object
{
	class CollisionClass : public ICollisionClass
	{
	public:
		CollisionClass() = default;
		explicit CollisionClass(float range);
		virtual ~CollisionClass() = default;

		virtual DirectX::ContainmentType GetCollideState(const AffineInfo& affine) override;
		virtual DirectX::ContainmentType GetCollideState(DirectX::XMFLOAT4 position, DirectX::XMFLOAT4 forward) override;

		virtual inline DirectX::XMFLOAT4 GetColor() const override { return m_Color; }
		virtual inline void SetColor(DirectX::XMFLOAT4 color) override { m_Color = color; }

		virtual inline std::unique_ptr<IObjectClass> Clone() const override;

	private:
		DirectX::XMFLOAT4 m_Color = DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f);
		float m_Range = 10.f;
	};
}