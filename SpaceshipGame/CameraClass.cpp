#include "pch.h"
#include "TransformClass.h"
#include "CameraClass.h"

static ErrorContent e;

CameraClass::CameraClass(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT4& scaling)
{
	HRESULT result = S_OK;

	result = Initialize(position, rotation, scaling);
	if (FAILED(result))
	{
		Shutdown();
		throw e;
	}
}

CameraClass::~CameraClass()
{
	Shutdown();
}

HRESULT CameraClass::Initialize(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT4& scaling)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("CameraClass Initialize()");

	m_Transform = new TransformClass(position, rotation, scaling);
	if (!m_Transform)
	{
		e.contents = _T("TransformClass 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	Render();

	return result;
}

void CameraClass::Shutdown()
{
	if (m_Transform)
	{
		delete m_Transform;
		m_Transform = nullptr;
	}
}

void CameraClass::MoveForward()
{
	using namespace DirectX;

	XMFLOAT4 param;
	XMVECTOR position = XMLoadFloat4(&(m_Transform->GetPosition()));
	position += m_ForwardVector * m_MoveSpeed;

	XMStoreFloat4(&param, position);
	m_Transform->SetPosition(param);
}

void CameraClass::MoveBackward()
{
	using namespace DirectX;

	XMFLOAT4 param;
	XMVECTOR position = XMLoadFloat4(&(m_Transform->GetPosition()));
	position -= m_ForwardVector * m_MoveSpeed;

	XMStoreFloat4(&param, position);
	m_Transform->SetPosition(param);
}

void CameraClass::MoveLeft()
{
	using namespace DirectX;

	XMFLOAT4 param;
	XMVECTOR position = XMLoadFloat4(&(m_Transform->GetPosition()));
	position -= m_RightVector * m_MoveSpeed;

	XMStoreFloat4(&param, position);
	m_Transform->SetPosition(param);
}

void CameraClass::MoveRight()
{
	using namespace DirectX;

	XMFLOAT4 param;
	XMVECTOR position = XMLoadFloat4(&(m_Transform->GetPosition()));
	position += m_RightVector * m_MoveSpeed;

	XMStoreFloat4(&param, position);
	m_Transform->SetPosition(param);
}

HRESULT CameraClass::Move(const MoveState& dir)
{
	e.title = _T("CameraClass Initialize()");

	switch (dir)
	{
	case MoveState::MOVE_FORWARD:
		MoveForward();
		break;
	case MoveState::MOVE_BACKWARD:
		MoveBackward();
		break;
	case MoveState::MOVE_LEFT:
		MoveLeft();
		break;
	case MoveState::MOVE_RIGHT:
		MoveRight();
		break;
	default:
		e.contents = _T("예상치 못한 동작입니다.");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	return S_OK;
}

void CameraClass::Render()
{
	float pitch, yaw, roll;
	DirectX::XMMATRIX RotationMatrix;
	DirectX::XMVECTOR position, target;

	// 회전 행렬 설정 //
	pitch = m_Transform->GetRotation().x;
	yaw = m_Transform->GetRotation().y;
	roll = m_Transform->GetRotation().z;
	RotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// 카메라의 local 좌표계의 축 추출 //
	m_ForwardVector = DirectX::XMVector3Normalize(RotationMatrix.r[2]);
	m_RightVector = DirectX::XMVector3Normalize(RotationMatrix.r[0]);
	m_UpVector = DirectX::XMVector3Normalize(RotationMatrix.r[1]);

	// 카메라의 world 좌표계의 position 추출 //
	position = DirectX::XMLoadFloat4(&(m_Transform->GetPosition()));

	// target vector 계산 //
	target = DirectX::XMVectorAdd(position, m_ForwardVector);

	// view matrix 생성 //
	m_ViewMatrix = DirectX::XMMatrixLookAtLH(position, target, m_UpVector);
}
