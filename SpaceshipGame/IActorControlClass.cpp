#include "pch.h"
#include "ActorClass.h"
#include "IAffineControlClass.h"
#include "ICollisionContorlClass.h"
#include "IActorControlClass.h"

static ErrorContent e;

IActorControlClass::IActorControlClass(const AffineInfo& ModelAffine, const AffineInfo& CollisionAffine, const ModelIDs ModelID)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("IActorControlClass constructor");

	result = Initailize(ModelAffine, CollisionAffine, ModelID);
	if (FAILED(result))
	{
		Shutdown();
		throw e;
	}
}

IActorControlClass::~IActorControlClass()
{
	Shutdown();
}

void IActorControlClass::Move(const MoveState& state, const float& frame_time, const bool& IsKeyDown)
{
	m_Actor->GetAffine()->MoveActor(state, frame_time, IsKeyDown);
	m_Actor->GetCollision()->MoveCollision(state, frame_time, IsKeyDown);
}

void IActorControlClass::Rotate(const long& MouseX, const long& MouseY, const float& frame_time, const bool& IsKeyDown)
{
	m_Actor->GetAffine()->RotateActor(MouseX, MouseY, frame_time, IsKeyDown);
	m_Actor->GetCollision()->RotateCollision(MouseX, MouseY, frame_time, IsKeyDown);
}

const DirectX::ContainmentType& IActorControlClass::GetCollideState(const DirectX::BoundingOrientedBox* const& collision)
{
	return m_Actor->GetCollision()->GetCollideState(collision);
}

const DirectX::ContainmentType& IActorControlClass::GetCollideState(const DirectX::SimpleMath::Ray* const& ray)
{
	return m_Actor->GetCollision()->GetCollideState(ray);
}

inline const DirectX::BoundingOrientedBox* const& IActorControlClass::GetCollision() const
{
	return m_Actor->GetCollision()->GetCollision();
}

inline const DirectX::XMFLOAT4X4& IActorControlClass::GetActorAffineMatrix() const
{
	return m_Actor->GetAffine()->GetAffineMatrix();
}

inline const DirectX::XMFLOAT4X4& IActorControlClass::GetCollisionAffineMatrix() const
{
	return m_Actor->GetCollision()->GetAffineMatrix();
}

HRESULT IActorControlClass::Initailize(const AffineInfo& ModelAffine, const AffineInfo& CollisionAffine, const ModelIDs ModelID)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("IActorControlClass Initailize()");

	// IAffineControlClass 인스턴스 생성 //
	m_Actor = new ActorClass(ModelAffine, CollisionAffine, ModelID);
	if (!m_Actor)
	{
		e.contents = _T("ActorClass 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	return result;
}

void IActorControlClass::Shutdown()
{
	if (m_Actor)
	{
		delete m_Actor;
		m_Actor = nullptr;
	}
}
