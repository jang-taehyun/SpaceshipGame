#include "pch.h"
#include "AffineClass.h"
#include "IAffineControlClass.h"

static ErrorContent e;

IAffineControlClass::IAffineControlClass(const AffineInfo& affine)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("IAffineControlClass constructor");

	result = Initailize(affine);
	if (FAILED(result))
	{
		Shutdown();
		throw e;
	}
}

IAffineControlClass::~IAffineControlClass()
{
	Shutdown();
}

DirectX::XMFLOAT4X4& IAffineControlClass::GetAffineMatrix() const
{
	using namespace DirectX;

	XMFLOAT3 convert;
	XMVECTOR VPosition, VRotation, VScaling;
	XMMATRIX MPosition, MRotation, MScaling, MAffine;
	XMFLOAT4X4 ret;

	// XMFLOAT4 타입을 XMVECTOR 타입으로 변환 후, affine matrix 연산 //
	// 변환 과정 : XMFLOAT4 -> XMVECTOR -> XMMATRIX
	VPosition = XMLoadFloat4(&(m_affine->GetPosition()));
	VRotation = XMLoadFloat4(&(m_affine->GetRotation()));
	VScaling = XMLoadFloat4(&(m_affine->GetScaling()));

	MPosition = XMMatrixTranslationFromVector(VPosition);
	MRotation = XMMatrixRotationQuaternion(VRotation);
	MScaling = XMMatrixScalingFromVector(VScaling);

	MAffine = MScaling * MRotation * MPosition;

	// XMMATRIX 타입을 XMFLOAT4X4 타입으로 변환 후 반환 //
	XMStoreFloat4x4(&ret, MAffine);
	return ret;
}

void IAffineControlClass::MoveActor(const MoveState& state, const float& frame_time, const bool& IsKeyDown)
{
	float pitch, yaw, roll;
	DirectX::XMMATRIX RotationMatrix;
	DirectX::XMVECTOR forward, right, up;
	DirectX::XMFLOAT4 rotate = m_affine->GetRotation();
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
	}

	// position 데이터 업데이트 //
	m_affine->SetPosition(setter);
}

void IAffineControlClass::RotateActor(const long& MouseX, const long& MouseY, const float& frame_time, const bool& IsKeyDown)
{
	DirectX::XMFLOAT4 rot = m_affine->GetRotation();

	// speed 계산 //
	ComputeRotateSpeed(frame_time, IsKeyDown);

	// yaw, pitch 업데이트 //
	// yaw
	rot.y += ((float)MouseX * m_RotateSpeed);
	// pitch
	rot.x += ((float)MouseY * m_RotateSpeed);

	// 변경된 rotate 적용 //
	m_affine->SetRotation(rot);
}

HRESULT IAffineControlClass::Initailize(const AffineInfo& affine)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("IAffineControlClass Initailize()");

	m_affine = new AffineClass(affine);
	if (!m_affine)
	{
		e.contents = _T("AffineClass 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	return result;
}

void IAffineControlClass::Shutdown()
{
	if (m_affine)
	{
		delete m_affine;
		m_affine = nullptr;
	}
}

DirectX::XMFLOAT4& IAffineControlClass::MoveLeft(const DirectX::XMVECTOR& RightVector)
{
	using namespace DirectX;

	XMVECTOR position = XMLoadFloat4(&(m_affine->GetPosition()));
	XMFLOAT4 ret;

	// 최종 position 계산 및 반환 //
	position -= (RightVector * m_MoveSpeed);
	XMStoreFloat4(&ret, position);

	return ret;
}

DirectX::XMFLOAT4& IAffineControlClass::MoveRight(const DirectX::XMVECTOR& RightVector)
{
	using namespace DirectX;

	XMVECTOR position = XMLoadFloat4(&(m_affine->GetPosition()));
	XMFLOAT4 ret;

	// 최종 position 계산 및 반환 //
	position += (RightVector * m_MoveSpeed);
	XMStoreFloat4(&ret, position);

	return ret;
}

DirectX::XMFLOAT4& IAffineControlClass::MoveForward(const DirectX::XMVECTOR& ForwardVector)
{
	using namespace DirectX;

	XMVECTOR position = XMLoadFloat4(&(m_affine->GetPosition()));
	XMFLOAT4 ret;

	// 최종 position 계산 및 반환 //
	position += (ForwardVector * m_MoveSpeed);
	XMStoreFloat4(&ret, position);

	return ret;
}

DirectX::XMFLOAT4& IAffineControlClass::MoveBackward(const DirectX::XMVECTOR& ForwardVector)
{
	using namespace DirectX;

	XMVECTOR position = XMLoadFloat4(&(m_affine->GetPosition()));
	XMFLOAT4 ret;

	// 최종 position 계산 및 반환 //
	position -= (ForwardVector * m_MoveSpeed);
	XMStoreFloat4(&ret, position);
	
	return ret;
}

void IAffineControlClass::ComputeMoveSpeed(const float& frame_time, const bool& IsKeyDown)
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

void IAffineControlClass::ComputeRotateSpeed(const float& frame_time, const bool& IsKeyDown)
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
