#pragma once

/**
* CameraClass °³¿ä
*/

#include "ObjectClass.h"

namespace Object
{
	class CameraClass : public ObjectClass
	{
	public:
		CameraClass() = default;
		virtual ~CameraClass() = default;

		DirectX::XMFLOAT4X4 Render() const;

		virtual inline std::unique_ptr<IObjectClass> Clone() const override { return std::make_unique<CameraClass>(*this); }
	};
}