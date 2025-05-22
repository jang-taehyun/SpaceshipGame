#pragma once

/**
* IObjectClass 개요
* - GameObjectClass의 interface
* - GameObjectClass 내의 공통 기능 선언
*/

class IObjectClass
{
public:
	virtual ~IObjectClass() = default;

	// affine matrxi(world matrix) 반환
	virtual const DirectX::XMFLOAT4X4& GetAffineMatrix() const = 0;

	// position 반환
	virtual inline const DirectX::XMFLOAT4& GetPosition() const = 0;

	// rotation 반환
	virtual inline const DirectX::XMFLOAT4& GetRotation() const = 0;

	// scale 반환
	virtual inline const DirectX::XMFLOAT4& GetScale() const = 0;

	// 방향 벡터(forward vector, right vector, up vector) 반환
	virtual void GetDirectionVectors(DirectX::XMFLOAT4& forward, DirectX::XMFLOAT4& right, DirectX::XMFLOAT4& up) const = 0;

	// forward vector 반환
	virtual const DirectX::XMFLOAT4& GetForwardVector() const = 0;

	// right vector 반환
	virtual const DirectX::XMFLOAT4& GetRightVector() const = 0;

	// up vector 반환
	virtual const DirectX::XMFLOAT4& GetUpVector() const = 0;

	virtual inline void SetPosition(const DirectX::XMFLOAT4& pos) = 0;							// position 대입
	virtual inline void SetPosition(const float& x, const float& y, const float& z) = 0;		// position 대입
	virtual inline void SetPosition(const float&& x, const float&& y, const float&& z) = 0;		// position 대입
	
	virtual inline void SetRotation(const DirectX::XMFLOAT4& rot) = 0;							// rotation 대입
	virtual inline void SetRotation(const float& x, const float& y, const float& z) = 0;		// rotation 대입
	virtual inline void SetRotation(const float&& x, const float&& y, const float&& z) = 0;		// rotation 대입
	
	virtual inline void SetScale(const DirectX::XMFLOAT4& scale) = 0;							// scale 대입
	virtual inline void SetScale(const float& x, const float& y, const float& z) = 0;			// scale 대입
	virtual inline void SetScale(const float&& x, const float&& y, const float&& z) = 0;		// scale 대입

private:

	// 멤버 변수, 리소스 해제
	virtual void Shutdown() = 0;
};

