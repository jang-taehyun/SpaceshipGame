#pragma once

/**
* ICollisionClass °³¿ä
*/

class ICollisionClass
{
public:
	virtual ~ICollisionClass() = default;

	virtual const DirectX::XMFLOAT4& GetPosition() = 0;
	virtual void SetPosition(const DirectX::XMFLOAT4 pos) = 0;
};

