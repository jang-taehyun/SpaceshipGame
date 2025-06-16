#include "pch.h"
#include "ActorClass.h"
#include "ActorManagerClass.h"

bool ActorManagerClass::IsInitialize = false;
static ErrorContent e;

ActorManagerClass::ActorManagerClass(const AffineInfo* ActorAffines, const AffineInfo* CollisionAffines, const ModelIDs* ModelIDs, int ActorCount, int PlayerIdx) : m_ActorCount(ActorCount), m_PlayerIdx(PlayerIdx)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("ActorManagerClass Constructor");

	// ActorManagerClass 인스턴스가 이미 존재하는지 검사 //
	if (IsInitialize)
	{
		e.contents = _T("이미 ActorManagerClass 인스턴스가 존재합니다.");
		e.errorCode = E_FAIL;
		throw e;
	}

	// actor 인터페이스 생성 개수 검사 //
	if (ActorCount >= ACTOR_MAX_COUNT)
	{
		e.contents = _T("actor 인터페이스 개수가 ACTOR_MAX_COUNT보다 더 큽니다.");
		e.errorCode = E_FAIL;
		throw e;
	}

	// 초기화 //
	result = Initialize(ActorAffines, CollisionAffines, ModelIDs);
	if (FAILED(result))
	{
		throw e;
	}

	IsInitialize = true;
}

ActorManagerClass::~ActorManagerClass()
{
	IsInitialize = false;
}

const IObjectClass* ActorManagerClass::operator[](int idx) const
{
	if (idx >= m_ActorCount)
		return nullptr;
	return m_ActorInterfaces[idx].get();
}

HRESULT ActorManagerClass::Initialize(const AffineInfo* ActorAffines, const AffineInfo* CollisionAffines, const ModelIDs* ModelIDs)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("ActorManagerClass Initialize()");

	// Actor 인터페이스 생성 //
	for (int i = 0; i < m_ActorCount; ++i)
	{	
		m_ActorInterfaces[i] = std::make_unique<ActorClass>(ActorAffines[i], CollisionAffines[i], ModelIDs[i]);
		if (!m_ActorInterfaces[i])
		{
			e.contents = _T("ActorClass 인스턴스 생성 실패");
			e.errorCode = E_FAIL;
			return E_FAIL;
		}
	}

	return result;
}
