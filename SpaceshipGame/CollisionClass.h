#pragma once

/**
* CollisionClass °³¿ä
*/

#include <DirectXCollision.h>
#include "ObjectClass.h"

namespace Object
{
	class CollisionClass : public ObjectClass
	{
	public:
		CollisionClass() = default;
		virtual ~CollisionClass() = default;

		virtual DirectX::ContainmentType GetCollideState(const AffineInfo& affine);
		virtual DirectX::ContainmentType GetCollideState(DirectX::XMFLOAT4 position, DirectX::XMFLOAT4 forward);

		virtual inline std::unique_ptr<IObjectClass> Clone() const override { return std::make_unique<CollisionClass>(*this); }

	private:
		float m_Range = 0.f;

#ifdef DEBUG
	public:
		inline Graphic::Model::ID GetModelID() const { return m_ModelID; }
		inline DirectX::XMFLOAT4 GetColor() const { return m_Color; }
		inline void SetColor(DirectX::XMFLOAT4 color) { m_Color = color; }

	private:
		Graphic::Model::ID m_ModelID = Graphic::Model::ID::COLLISION;
		DirectX::XMFLOAT4 m_Color = DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f);
#endif // DEBUG

	};
}