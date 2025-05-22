#include "pch.h"
#include "IAffineControlClass.h"
#include "ICollisionContorlClass.h"
#include "ActorClass.h"

static ErrorContent e;

ActorClass::ActorClass(const AffineInfo& ModelAffine, const AffineInfo& CollisionAffine, const ModelIDs ModelID = ModelIDs::DEFAULT_SPACESHIP) : m_ModelID(ModelID)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("ActorClass constructor");

	result = Initailize(ModelAffine, CollisionAffine, ModelID);
	if (FAILED(result))
	{
		Shutdown();
		throw e;
	}
}

ActorClass::~ActorClass()
{
	Shutdown();
}

HRESULT ActorClass::Initailize(const AffineInfo& ModelAffine, const AffineInfo& CollisionAffine, const ModelIDs ModelID = ModelIDs::DEFAULT_SPACESHIP)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("ActorClass Initailize()");

	// actor의 affine 관련 인터페이스(m_AffineInterface) 생성 //
	m_AffineInterface = new IAffineControlClass(ModelAffine);
	if (!m_AffineInterface)
	{
		e.contents = _T("IAffineControlClass 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}
	
	// collision 관련 인터페이스 생성 //
	m_CollisionInterface = new ICollisionContorlClass(CollisionAffine);
	if (!m_CollisionInterface)
	{
		e.contents = _T("ICollisionContorlClass 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	return result;
}

void ActorClass::Shutdown()
{
	if (m_CollisionInterface)
	{
		ICollisionContorlClass* tmp = static_cast<ICollisionContorlClass*>(m_CollisionInterface);

		delete tmp;

		tmp = nullptr;
		m_CollisionInterface = nullptr;
	}

	if (m_AffineInterface)
	{
		delete m_AffineInterface;
		m_AffineInterface = nullptr;
	}
}