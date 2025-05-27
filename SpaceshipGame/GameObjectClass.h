#pragma once

/**
* GameObjectClass 개요
* - affine 관련 데이터(position, rotate, scale) 관리
* - affine 관련 기능 정의
*/

#include "IObjectClass.h"

class GameObjectClass : public IObjectClass
{
public:
	virtual ~GameObjectClass() = default;

	// affine matrxi(world matrix) 반환
	virtual const DirectX::XMFLOAT4X4& GetAffineMatrix() const override;

	// position 반환
	virtual inline const DirectX::XMFLOAT4& GetPosition() const override { return m_Position; }

	// rotation 반환
	virtual inline const DirectX::XMFLOAT4& GetRotation() const override { return m_Rotation; }

	// scale 반환
	virtual inline const DirectX::XMFLOAT4& GetScale() const override { return m_Scale; }

	virtual inline void SetPosition(const DirectX::XMFLOAT4& pos) override { m_Position = pos; };													// position 대입
	virtual inline void SetPosition(const float x, const float y, const float z) override { m_Position = DirectX::XMFLOAT4(x, y, z, 1.f); }			// position 대입

	virtual inline void SetRotation(const DirectX::XMFLOAT4& rot) override { m_Rotation = rot; }													// rotation 대입
	virtual inline void SetRotation(const float x, const float y, const float z) override { m_Rotation = DirectX::XMFLOAT4(x, y, z, 1.f); }			// rotation 대입

	virtual inline void SetScale(const DirectX::XMFLOAT4& scale) override { m_Scale = scale; }														// scale 대입
	virtual inline void SetScale(const float x, const float y, const float z) override { m_Scale = DirectX::XMFLOAT4(x, y, z, 1.f); }				// scale 대입

private:

	// 멤버 변수, 리소스 해제
	virtual void Shutdown() = 0;

private:
	DirectX::XMFLOAT4 m_Position = { 0.f, 0.f, 0.f, 1.f };
	DirectX::XMFLOAT4 m_Rotation = { 1.f, 1.f, 1.f, 1.f };
	DirectX::XMFLOAT4 m_Scale = { 1.f, 1.f, 1.f, 1.f };
};

