#include "pch.h"
#include "PlayerClass.h"

static ErrorContent e;

PlayerClass::PlayerClass(const AffineInfo& affine, const ModelIDs ModelID) : ActorClass(affine, ModelID)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("PlayerClass constructor");

	result = IntializeRay(affine);
	if (FAILED(result))
	{
		ShutdownRay();
		throw e;
	}
}

PlayerClass::~PlayerClass()
{
	ShutdownRay();
}

HRESULT PlayerClass::IntializeRay(const AffineInfo& affine)
{
	HRESULT result = S_OK;
	DirectX::SimpleMath::Vector3 pos, dir;

	// 에러 메세지, 구조체 초기화 //
	e.title = _T("PlayerClass IntializeRay()");
	pos.x = affine.position.x;
	pos.y = affine.position.y;
	pos.z = affine.position.z;
	dir.z = 1.f;

	m_Ray = new DirectX::SimpleMath::Ray(pos, dir);
	if (!m_Ray)
	{
		e.contents = _T("DirectX::SimpleMath::Ray 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	return result;
}

void PlayerClass::ShutdownRay()
{
	if (m_Ray)
	{
		delete m_Ray;
		m_Ray = nullptr;
	}
}
