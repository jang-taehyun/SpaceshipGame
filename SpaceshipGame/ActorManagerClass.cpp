#include "pch.h"
#include "IActorControlClass.h"
#include "ActorManagerClass.h"

bool ActorManagerClass::IsInitialize = false;
static ErrorContent e;

ActorManagerClass::ActorManagerClass(const AffineInfo* const& ActorAffines, const AffineInfo* const& CollisionAffines, const ModelIDs* const& ModelIDs, const int& ActorCount, const int& PlayerIdx)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("ActorManagerClass Constructor");

	if (IsInitialize)
	{
		e.contents = _T("이미 ActorManagerClass 인스턴스가 존재합니다.");
		e.errorCode = E_FAIL;
		throw e;
	}

	result = Initialize(ActorAffines, CollisionAffines, ModelIDs, ActorCount, PlayerIdx);
	if (FAILED(result))
	{
		Shutdown();
		throw e;
	}

	IsInitialize = true;
}

ActorManagerClass::~ActorManagerClass()
{
	Shutdown();
	IsInitialize = false;
}

IActorControlClass* const& ActorManagerClass::operator()(int idx) const
{
	if (idx >= m_ActorCount)
		return nullptr;
	return m_Actors[idx];
}

HRESULT ActorManagerClass::Initialize(const AffineInfo* const& ActorAffines, const AffineInfo* const& CollisionAffines, const ModelIDs* const& ModelIDs, const int& ActorCount, const int& PlayerIdx)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("GraphicsClass Initialize()");

	// Other 객체 생성 및 초기화 //
	if (ActorCount >= ACTOR_MAX_COUNT)
	{
		e.contents = _T("OthersCount 매개변수의 값이 OtherMaxLimit보다 더 큽니다.");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	// 멤버 변수 초기화 //
	m_ActorCount = ActorCount;
	m_PlayerIdx = PlayerIdx;
	for (int i = 0; i < m_ActorCount; ++i)
	{	
		m_Actors[i] = new ActorClass(Affines[i], ModelIDs[i]);
		if (!m_Actors[i])
		{
			e.contents = _T("ActorClass 인스턴스 생성 실패");
			e.errorCode = E_FAIL;
			return E_FAIL;
		}
	}

	return result;
}

void ActorManagerClass::Shutdown()
{
	PlayerClass* player = nullptr;

	for (int i = 0; i < m_ActorCount; ++i)
	{
		if (m_Actors[i])
		{
			if (i == m_PlayerIdx)
			{
				player = static_cast<PlayerClass*>(m_Actors[i]);
				delete player;
				player = nullptr;
			}
			else
				delete m_Actors[i];

			m_Actors[i] = nullptr;
		}
	}
}
