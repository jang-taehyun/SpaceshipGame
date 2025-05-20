#include "pch.h"
#include "CollisionClass.h"
#include "IAffineControlClass.h"
#include "ICollisionContorlClass.h"

static ErrorContent e;

ICollisionContorlClass::ICollisionContorlClass(const AffineInfo& affine)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("ICollisionContorlClass constructor");

	result = Initialize(affine);
	if (FAILED(result))
	{
		Shutdown();
		throw e;
	}
}

ICollisionContorlClass::~ICollisionContorlClass()
{
	Shutdown();
}

const DirectX::ContainmentType& ICollisionContorlClass::GetCollideState(const DirectX::BoundingOrientedBox* const& collision)
{
	DirectX::ContainmentType ret = m_Collision->GetCollision()->Contains(*collision);
	return ret;
}

const DirectX::ContainmentType& ICollisionContorlClass::GetCollideState(const DirectX::SimpleMath::Ray* const& ray)
{
	float dist = 5.f;
	DirectX::XMVECTOR origin;
	DirectX::XMVECTOR direction;
	bool IsHit = false;

	// 매개 변수 검사 //
	if (!ray)
		return DirectX::ContainmentType::DISJOINT;

	// ray의 position, direction 검사
	origin = DirectX::XMLoadFloat3(&(ray->position));
	direction = DirectX::XMLoadFloat3(&(ray->direction));
	if (DirectX::XMVector3Equal(direction, DirectX::XMVectorZero()))
		return DirectX::ContainmentType::DISJOINT;

	// 충돌 검사 //
	direction = DirectX::XMVector3Normalize(direction);
	IsHit = m_Collision->GetCollision()->Intersects(origin, direction, dist);

	return (IsHit ? DirectX::ContainmentType::CONTAINS : DirectX::ContainmentType::DISJOINT);
}

inline const DirectX::BoundingOrientedBox* const& ICollisionContorlClass::GetCollision() const
{
	return m_Collision->GetCollision();
}

DirectX::XMFLOAT4X4& ICollisionContorlClass::GetAffineMatrix() const
{
	using namespace DirectX;

	XMFLOAT3 convert;
	XMFLOAT4 F4Position, F4Rotation, F4Scaling;
	XMVECTOR VPosition, VRotation, VScaling;
	XMMATRIX MPosition, MRotation, MScaling, MAffine;
	XMFLOAT4X4 ret;

	// OBB 구조체에 있는 변수를 XMFLOAT4 타입으로 통일 후 초기화 //
	F4Rotation = m_Collision->GetCollision()->Orientation;

	convert = m_Collision->GetCollision()->Center;
	F4Position = XMFLOAT4(convert.x, convert.y, convert.z, 1.f);

	convert = m_Collision->GetCollision()->Extents;
	F4Scaling = XMFLOAT4(convert.x, convert.y, convert.z, 1.f);

	// XMFLOAT4 타입을 XMVECTOR 타입으로 변환 후, affine matrix 연산 //
	// 변환 과정 : XMFLOAT4 -> XMVECTOR -> XMMATRIX
	VPosition = XMLoadFloat4(&F4Position);
	VRotation = XMLoadFloat4(&F4Rotation);
	VScaling = XMLoadFloat4(&F4Scaling);

	MPosition = XMMatrixTranslationFromVector(VPosition);
	MRotation = XMMatrixRotationQuaternion(VRotation);
	MScaling = XMMatrixScalingFromVector(VScaling);

	MAffine = MScaling * MRotation * MPosition;

	// XMMATRIX 타입을 XMFLOAT4X4 타입으로 변환 후 반환 //
	XMStoreFloat4x4(&ret, MAffine);
	return ret;
}

void ICollisionContorlClass::MoveCollision(const MoveState& state, const float& frame_time, const bool& IsKeyDown)
{
	float pitch, yaw, roll;
	DirectX::XMMATRIX RotationMatrix;
	DirectX::XMVECTOR forward, right, up;
	DirectX::XMFLOAT4 rotate = m_Collision->GetCollision()->Orientation;
	DirectX::XMFLOAT4 setter;

	// 해당 객체의 local space의 rotate matrix를 이용해, forward, right, up vector 계산 //
	// rotate matrix 생성
	pitch = rotate.x;
	yaw = rotate.y;
	roll = rotate.z;
	RotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// 해당 객체의 local 좌표계의 축(forward, right, up vector) 추출
	forward = DirectX::XMVector3Normalize(RotationMatrix.r[2]);
	right = DirectX::XMVector3Normalize(RotationMatrix.r[0]);
	up = DirectX::XMVector3Normalize(RotationMatrix.r[1]);

	// move speed 계산 //
	ComputeMoveSpeed(frame_time, IsKeyDown);

	// actor 이동 //
	switch (state)
	{
	case MoveState::MOVE_FORWARD:
		setter = MoveForward(forward);
		break;
	case MoveState::MOVE_BACKWARD:
		setter = MoveBackward(forward);
		break;
	case MoveState::MOVE_LEFT:
		setter = MoveLeft(right);
		break;
	case MoveState::MOVE_RIGHT:
		setter = MoveRight(right);
		break;
	default:
		setter = rotate;
		break;
	}

	// position 데이터 업데이트 //
	m_Collision->GetCollision()->Orientation = setter;
}

void ICollisionContorlClass::RotateCollision(const long& MouseX, const long& MouseY, const float& frame_time, const bool& IsKeyDown)
{
	DirectX::XMFLOAT4 rot = m_Collision->GetCollision()->Orientation;

	// speed 계산 //
	ComputeRotateSpeed(frame_time, IsKeyDown);

	// yaw, pitch 업데이트 //
	// yaw
	rot.y += ((float)MouseX * m_RotateSpeed);
	// pitch
	rot.x += ((float)MouseY * m_RotateSpeed);

	// 변경된 rotate 적용 //
	m_Collision->GetCollision()->Orientation = rot;
}

HRESULT ICollisionContorlClass::Initialize(const AffineInfo& affine)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("ICollisionContorlClass Initialize()");

	// CollisionClass 인스턴스 생성 //
	m_Collision = new CollisionClass(affine);
	if (!m_Collision)
	{
		e.contents = _T("CollisionClass 객체 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	return result;
}

void ICollisionContorlClass::Shutdown()
{
	if (m_Collision)
	{
		delete m_Collision;
		m_Collision = nullptr;
	}
}

DirectX::XMFLOAT4& ICollisionContorlClass::MoveLeft(const DirectX::XMVECTOR& RightVector)
{
	using namespace DirectX;

	XMFLOAT3 convert = m_Collision->GetCollision()->Center;
	XMFLOAT4 pos = XMFLOAT4(convert.x, convert.y, convert.z, 1.f);
	XMVECTOR position = XMLoadFloat4(&(pos));
	XMFLOAT4 ret;

	// 최종 position 계산 및 반환 //
	position -= (RightVector * m_MoveSpeed);
	XMStoreFloat4(&ret, position);

	return ret;
}

DirectX::XMFLOAT4& ICollisionContorlClass::MoveRight(const DirectX::XMVECTOR& RightVector)
{
	using namespace DirectX;

	XMFLOAT3 convert = m_Collision->GetCollision()->Center;
	XMFLOAT4 pos = XMFLOAT4(convert.x, convert.y, convert.z, 1.f);
	XMVECTOR position = XMLoadFloat4(&(pos));
	XMFLOAT4 ret;

	// 최종 position 계산 및 반환 //
	position += (RightVector * m_MoveSpeed);
	XMStoreFloat4(&ret, position);

	return ret;
}

DirectX::XMFLOAT4& ICollisionContorlClass::MoveForward(const DirectX::XMVECTOR& ForwardVector)
{
	using namespace DirectX;

	XMFLOAT3 convert = m_Collision->GetCollision()->Center;
	XMFLOAT4 pos = XMFLOAT4(convert.x, convert.y, convert.z, 1.f);
	XMVECTOR position = XMLoadFloat4(&(pos));
	XMFLOAT4 ret;

	// 최종 position 계산 및 반환 //
	position += (ForwardVector * m_MoveSpeed);
	XMStoreFloat4(&ret, position);

	return ret;
}

DirectX::XMFLOAT4& ICollisionContorlClass::MoveBackward(const DirectX::XMVECTOR& ForwardVector)
{
	using namespace DirectX;

	XMFLOAT3 convert = m_Collision->GetCollision()->Center;
	XMFLOAT4 pos = XMFLOAT4(convert.x, convert.y, convert.z, 1.f);
	XMVECTOR position = XMLoadFloat4(&(pos));
	XMFLOAT4 ret;

	// 최종 position 계산 및 반환 //
	position -= (ForwardVector * m_MoveSpeed);
	XMStoreFloat4(&ret, position);

	return ret;
}

void ICollisionContorlClass::ComputeMoveSpeed(const float& frame_time, const bool& IsKeyDown)
{
	if (IsKeyDown)
	{
		m_MoveSpeed += (frame_time * m_KeyboardSensitivity);

		if (m_MoveSpeed > (frame_time * m_KeyboardSensitivity))
		{
			m_MoveSpeed = frame_time * m_KeyboardSensitivity;
		}
	}
	else
	{
		m_MoveSpeed -= (frame_time * m_KeyboardSensitivity);

		if (m_MoveSpeed < 0.f)
		{
			m_MoveSpeed = 0.f;
		}
	}
}

void ICollisionContorlClass::ComputeRotateSpeed(const float& frame_time, const bool& IsKeyDown)
{
	// speed 계산 //
	if (IsKeyDown)
	{
		m_RotateSpeed += (frame_time * m_MouseSensitivity);

		if (m_RotateSpeed > (frame_time * m_KeyboardSensitivity))
		{
			m_RotateSpeed = frame_time * m_KeyboardSensitivity;
		}
	}
	else
	{
		m_RotateSpeed -= (frame_time * m_MouseSensitivity);

		if (m_RotateSpeed < 0.f)
		{
			m_RotateSpeed = 0.f;
		}
	}
}