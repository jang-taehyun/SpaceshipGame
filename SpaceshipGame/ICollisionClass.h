#pragma once

/**
* ICollisionClass °³¿ä
*/

#include <DirectXCollision.h>
#include "GameObjectClass.h"

namespace Object
{
	class ICollisionClass : public GameObjectClass
	{
	public:
		virtual ~ICollisionClass() = default;

		virtual DirectX::ContainmentType GetCollideState(const AffineInfo& affine) = 0;
		virtual DirectX::ContainmentType GetCollideState(DirectX::XMFLOAT4 position, DirectX::XMFLOAT4 forward) = 0;

		virtual inline DirectX::XMFLOAT4 GetColor() const = 0;
		virtual inline void SetColor(DirectX::XMFLOAT4 color) = 0;

		virtual inline std::unique_ptr<IObjectClass> Clone() const = 0;
	};

}