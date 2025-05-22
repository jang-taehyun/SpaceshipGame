#pragma once

/**
* AffineClass 개요
* - affine 관련 데이터(position, rotate, scale) 관리
*/

class AffineClass
{
public:
	explicit AffineClass(const AffineInfo& affine);
	virtual ~AffineClass() = default;

	// Getter //

	inline const DirectX::XMFLOAT4& GetPosition() const { return m_Position; }
	inline const DirectX::XMFLOAT4& GetRotation() const { return m_Rotation; }
	inline const DirectX::XMFLOAT4& GetScaling() const { return m_Scaling; }

	void GetDirectionVectors(DirectX::XMVECTOR& forward, DirectX::XMVECTOR& right, DirectX::XMVECTOR& up);

	const DirectX::XMFLOAT4& GetForwardVector() const;
	const DirectX::XMFLOAT4& GetRightVector() const;
	const DirectX::XMFLOAT4& GetUpVector() const;

	const DirectX::XMFLOAT4X4& GetAffine() const;

	// Setter //

	inline void SetPosition(const DirectX::XMFLOAT4& pos) { m_Position = pos; }
	inline void SetPosition(const float& x, const float& y, const float& z) { m_Position = DirectX::XMFLOAT4(x, y, z, 1.f); }

	inline void SetRotation(const DirectX::XMFLOAT4& rot) { m_Rotation = rot; }
	inline void SetRotation(const float& x, const float& y, const float& z) { m_Rotation = DirectX::XMFLOAT4(x, y, z, 1.f); }

	inline void SetScale(const DirectX::XMFLOAT4& scale) { m_Scaling = scale; }
	inline void SetScale(const float& x, const float& y, const float& z) { m_Scaling = DirectX::XMFLOAT4(x, y, z, 1.f); }
	
private:
	void Initialize(const AffineInfo& affine);

private:
	DirectX::XMFLOAT4 m_Position = { 0.f, 0.f, 0.f, 1.f };
	DirectX::XMFLOAT4 m_Rotation = { 1.f, 1.f, 1.f, 1.f };
	DirectX::XMFLOAT4 m_Scaling = { 1.f, 1.f, 1.f, 1.f };

public:
	AffineClass() = delete;
	AffineClass(const AffineClass& other) = delete;
};

