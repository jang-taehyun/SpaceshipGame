#include "pch.h"
#include "AffineClass.h"
#include "IObjectMoveableClass.h"
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

const DirectX::XMFLOAT4X4& IAffineControlClass::GetAffineMatrix() const
{
	return m_affine->GetAffine();
}

void IAffineControlClass::Move(const MoveState& state, const float& frame_time, const bool& IsKeyDown)
{
	DirectX::XMFLOAT4 vector;
	DirectX::XMFLOAT4 setter;

	// MoveState에 따라 방향 벡터 구하기 //
	// forward vector -> forward, backward
	// Right vector -> right, left
	// Up vector -> up, down
	switch (state)
	{
	case MoveState::MOVE_FORWARD:
	case MoveState::MOVE_BACKWARD:
		vector = m_affine->GetForwardVector();
		break;
	case MoveState::MOVE_LEFT:
	case MoveState::MOVE_RIGHT:
		vector = m_affine->GetRightVector();
		break;
	default:
		vector = m_affine->GetUpVector();
		break;
	}

	setter = m_MoveableCommand->Move(m_affine->GetPosition(), vector, state, frame_time, IsKeyDown);
	m_affine->SetPosition(setter);
}

void IAffineControlClass::Rotate(const long& MouseX, const long& MouseY, const float& frame_time, const bool& IsKeyDown)
{
	DirectX::XMFLOAT4 setter = m_MoveableCommand->Rotate(m_affine->GetRotation(), MouseX, MouseY, frame_time, IsKeyDown);
	m_affine->SetRotation(setter);
}

inline const DirectX::XMFLOAT4& IAffineControlClass::GetPosition() const
{
	return m_affine->GetPosition();
}

inline const DirectX::XMFLOAT4& IAffineControlClass::GetRotation() const
{
	return m_affine->GetRotation();
}

inline const DirectX::XMFLOAT4& IAffineControlClass::GetScaling() const
{
	return m_affine->GetScaling();
}

HRESULT IAffineControlClass::Initailize(const AffineInfo& affine)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("IAffineControlClass Initailize()");

	// AffineClass 인스턴스 생성 //
	m_affine = new AffineClass(affine);
	if (!m_affine)
	{
		e.contents = _T("AffineClass 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	// object의 이동, 회전을 담당하는 IObjectMoveableClass 인스턴스 생성 //
	m_MoveableCommand = new IObjectMoveableClass;
	if (!m_MoveableCommand)
	{
		e.contents = _T("IObjectMoveableClass 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	return result;
}

void IAffineControlClass::Shutdown()
{
	if (m_MoveableCommand)
	{
		delete m_MoveableCommand;
		m_MoveableCommand = nullptr;
	}

	if (m_affine)
	{
		delete m_affine;
		m_affine = nullptr;
	}
}