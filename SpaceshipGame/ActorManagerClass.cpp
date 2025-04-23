#include "pch.h"
#include "ActorClass.h"
#include "ActorManagerClass.h"

#define DEFAULT_OTHER_OBJECT_COUNT 1;

bool ActorManagerClass::IsInitialize = false;
static ErrorContent e;

ActorManagerClass::ActorManagerClass()
{
	HRESULT result = S_OK;
	AffineInfo playerinfo;
	AffineInfo otherinfo[1];
	ModelIDs othermodel[1];
	int cnt = DEFAULT_OTHER_OBJECT_COUNT;

	// 에러 메세지, 변수 초기화 //
	e.title = _T("ActorManagerClass Constructor");

	playerinfo.position = { 0.f, 0.f, 0.f, 1.f };
	playerinfo.rotation = { 0.f, 0.f, 0.f, 1.f };
	playerinfo.scale = { 1.f, 1.f, 1.f, 1.f };

	otherinfo[0].position = { 0.f, 0.f, 0.f, 1.f };
	otherinfo[0].rotation = { 0.f, 0.f, 0.f, 1.f };
	otherinfo[0].scale = { 1.f, 1.f, 1.f, 1.f };

	othermodel[0] = ModelIDs::DEFAULT_SPACESHIP;

	if (IsInitialize)
	{
		e.contents = _T("이미 ActorManagerClass 인스턴스가 존재합니다.");
		e.errorCode = E_FAIL;
		throw e;
	}

	result = Initialize(playerinfo, ModelIDs::DEFAULT_SPACESHIP, otherinfo, cnt, othermodel);
	if (FAILED(result))
	{
		Shutdown();
		throw e;
	}

	IsInitialize = true;
}

ActorManagerClass::ActorManagerClass(const AffineInfo& PlayerAffine, const ModelIDs PlayerModelID, const AffineInfo* const& OtherAffine, const int& OthersCount, const ModelIDs* const& OtherModelIDs)
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

	result = Initialize(PlayerAffine, PlayerModelID, OtherAffine, OthersCount, OtherModelIDs);
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

ActorClass* const& ActorManagerClass::GetOtherObject(int idx) const
{
	if (idx >= OtherMaxLimit)
		return nullptr;
	return m_Other[idx];
}

HRESULT ActorManagerClass::Initialize(const AffineInfo& PlayerAffine, const ModelIDs PlayerModelID, const AffineInfo* const& OtherAffine, const int& OthersCount, const ModelIDs* const& OtherModelIDs)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("GraphicsClass Initialize()");

	// Player 객체 생성 및 초기화 //
	m_Player = new ActorClass(PlayerAffine, PlayerModelID);
	if (!m_Player)
	{
		e.contents = _T("ActorClass 인스턴스 생성 실패(Player)");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	// Other 객체 생성 및 초기화 //
	if (OthersCount >= OtherMaxLimit)
	{
		e.contents = _T("OthersCount 매개변수의 값이 OtherMaxLimit보다 더 큽니다.");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	for (int i = 0; i < OthersCount; ++i)
	{
		m_Other[i] = new ActorClass(OtherAffine[i], OtherModelIDs[i]);
		if (!m_Other[i])
		{
			e.contents = _T("ActorClass 인스턴스 생성 실패(Other)");
			e.errorCode = E_FAIL;
			return E_FAIL;
		}
	}

	m_OthersCount = OthersCount;

	return result;
}

void ActorManagerClass::Shutdown()
{
	if (m_Player)
	{
		delete m_Player;
		m_Player = nullptr;
	}

	for (int i = 0; i < OtherMaxLimit; ++i)
	{
		if (m_Other[i])
		{
			delete m_Other[i];
			m_Other[i] = nullptr;
		}
	}
}
