#include "pch.h"

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

void CameraClass::Render()
{
	DirectX::XMVECTOR position, target;
	DirectX::XMVECTOR forward, up;

	// 카메라의 local 좌표계의 축 추출 //
	forward = m_Transform->GetForwardVector();
	up = m_Transform->GetUpVector();

	// 카메라의 world 좌표계의 position 추출 //
	position = DirectX::XMLoadFloat4(&(m_Transform->GetPosition()));

	// target vector 계산 //
	target = DirectX::XMVectorAdd(position, forward);

	// view matrix 생성 //
	m_ViewMatrix = DirectX::XMMatrixLookAtLH(position, target, up);
}
