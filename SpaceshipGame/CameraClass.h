#pragma once

/**
* CameraClass °³¿ä
*/

#include "MoveableObjectClass.h"

namespace Object
{
	class CameraClass : public MoveableObjectClass
	{
	public:
		CameraClass() = default;
		virtual ~CameraClass() = default;

		DirectX::XMFLOAT4X4 Render() const;
	};
}