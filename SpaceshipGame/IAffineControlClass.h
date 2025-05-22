#pragma once

/**
* IAffineControlClass 俺夸
* - AffineClass狼 interface
* - object客 包访等 interface 包府
*/

class AffineClass;
class IObjectMoveableClass;

class IAffineControlClass
{
public:
	explicit IAffineControlClass(const AffineInfo& affine);
	virtual ~IAffineControlClass();

	inline const DirectX::XMFLOAT4X4& GetAffineMatrix() const;

	void Move(const MoveState& state, const float& frame_time, const bool& IsKeyDown);
	void Rotate(const long& MouseX, const long& MouseY, const float& frame_time, const bool& IsKeyDown);

protected:
	inline const DirectX::XMFLOAT4& GetPosition() const;
	inline const DirectX::XMFLOAT4& GetRotation() const;
	inline const DirectX::XMFLOAT4& GetScaling() const;

private:
	HRESULT Initailize(const AffineInfo& affine);
	void Shutdown();

private:
	AffineClass* m_affine = nullptr;
	IObjectMoveableClass* m_MoveableCommand = nullptr;

public:
	IAffineControlClass() = delete;
	IAffineControlClass(const IAffineControlClass& other) = delete;
};

