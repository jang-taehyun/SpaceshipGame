#include "pch.h"
#include "StartSceneClass.h"
#include "SceneManagerClass.h"

bool Scene::SceneManagerClass::IsInitialize = false;

Scene::SceneManagerClass::SceneManagerClass()
{
	assert(IsInitialize);

	m_Scene = std::make_unique<StartSceneClass>(SceneState::START, SceneState::INGAME);
	assert(m_Scene);

	IsInitialize = true;
}

Scene::SceneManagerClass::~SceneManagerClass()
{
	// 마우스 커서를 보이게 하고 위치를 중앙으로 되돌리기 //
	if (!m_IsShowingCursor)
		ShowCursor(true);
	SetCursorPos(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);

	IsInitialize = false;
}

HRESULT Scene::SceneManagerClass::Frame(const System::InputClass* input, float frame_time)
{
	HRESULT result = S_OK;

	if (m_Scene->IsSceneEnded())
		ChangeScene();

	m_Scene->Frame(input, frame_time);

	return result;
}

HRESULT Scene::SceneManagerClass::ChangeScene()
{
	HRESULT result = S_OK;
	SceneState cur = m_Scene->GetNextSceneState();
	SceneState next;

	// scene 인스턴스 해제 //
	m_Scene.reset();

	// 다음 scene에 맞는 scene 인스턴스 생성 //
	switch (cur)
	{
	case SceneState::START:
		next = SceneState::INGAME;
		m_Scene = std::make_unique<>(cur, next);
		break;
	case SceneState::INGAME:
		next = SceneState::INGAME;
		m_Scene = std::make_unique<StartSceneClass>(cur, next);
		break;
	default:
		assert(false);
	}
	assert(m_Scene);

	return result;
}

HRESULT Scene::SceneManagerClass::ProcessCamera(CameraClass* const& camera, const InputClass* const& input, const float& frame_time)
{
	HRESULT result = S_OK;

	// 에러 메세지, 변수 초기화 //
	e.title = _T("SceneManagerClass ProcessCamera()");

	// camera 객체의 frame time 갱신
	UpdateCameraFrameTime(camera, frame_time);

	// keyboard, mouse의 input에 따라 camera 객체의 affine 변경
	result = UpdateCameraAffine(camera, input);
	if (FAILED(result))
	{
		throw e;
		return result;
	}

	return result;
}

HRESULT Scene::SceneManagerClass::ProcessActor(ActorManagerClass* const& actor_manager, const InputClass* const& input, const float& frame_time)
{
	HRESULT result = S_OK;

	// 에러 메세지, 변수 초기화 //
	e.title = _T("SceneManagerClass ProcessActor()");

	UpdateObjectFrameTime(actor_manager, frame_time);

	result = UpdateActorAffine(actor_manager, input);
	if (FAILED(result))
	{
		throw e;
		return result;
	}

	result = UpdateActorCollisionState(actor_manager, input);
	if (FAILED(result))
	{
		throw e;
		return result;
	}

	return result;
}

void Scene::SceneManagerClass::ProcessSceneInfo()
{
	switch (m_SceneState)
	{
	case SceneState::MATCH:
		m_SceneString = _T("MATCH");
		break;
	case SceneState::LOADING:
		m_SceneString = _T("LOADING");
		break;
	case SceneState::INGAME:
		m_SceneString = _T("INGAME");
		break;
	default:
		m_SceneString = _T("NONE");
		break;
	}
}

void Scene::SceneManagerClass::UpdateCameraFrameTime(CameraClass* const& camera, const float& frame_time)
{
	camera->GetAffineObject()->SetFrameTime(frame_time);
}

HRESULT Scene::SceneManagerClass::UpdateCameraAffine(CameraClass* const& camera, const InputClass* const& input)
{
	HRESULT result = S_OK;
	bool KeyDown = false;
	long MouseX = 0, MouseY = 0;

	// 에러 메세지, 변수 초기화 //
	e.title = _T("SceneManagerClass UpdateCameraAffine()");

	// 이동 //
	// 앞
	KeyDown = input->IsWBottunPressed();
	result = camera->GetAffineObject()->ChangePosition(MoveState::MOVE_FORWARD, KeyDown);
	if (FAILED(result))
	{
		e.contents = _T("camera 객체의 transform 변경(FORWARD) 실패");
		e.errorCode = result;
		return result;
	}

	// 뒤
	KeyDown = input->IsSBottunPressed();
	result = camera->GetAffineObject()->ChangePosition(MoveState::MOVE_BACKWARD, KeyDown);
	if (FAILED(result))
	{
		e.contents = _T("camera 객체의 transform 변경(BACKWARD) 실패");
		e.errorCode = result;
		return result;
	}

	// 왼쪽
	KeyDown = input->IsABottunPressed();
	result = camera->GetAffineObject()->ChangePosition(MoveState::MOVE_LEFT, KeyDown);
	if (FAILED(result))
	{
		e.contents = _T("camera 객체의 transform 변경(LEFT) 실패");
		e.errorCode = result;
		return result;
	}

	// 오른쪽
	KeyDown = input->IsDBottunPressed();
	result = camera->GetAffineObject()->ChangePosition(MoveState::MOVE_RIGHT, KeyDown);
	if (FAILED(result))
	{
		e.contents = _T("camera 객체의 transform 변경(RIGHT) 진행 실패");
		e.errorCode = result;
		return result;
	}

	// 회전
	KeyDown = input->IsMouseCenterBottunPressed();
	if (KeyDown)
	{
		input->GetMouseMoveDelta(MouseX, MouseY);
		camera->GetAffineObject()->ChangeRotation(MouseX, MouseY);
	
		if (m_IsShowingCursor)
		{
			ShowCursor(false);
			m_IsShowingCursor = false;
		}
	}
	else
	{
		if (!m_IsShowingCursor)
		{
			m_IsShowingCursor = true;
			ShowCursor(true);
	
			// 마우스 커서 위치를 중앙으로 초기화 //
			SetCursorPos(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
		}
	}

	return result;
}

void Scene::SceneManagerClass::UpdateObjectFrameTime(ActorManagerClass* const& actor_manager, const float& frame_time)
{
	actor_manager->GetPlayerObject()->GetAffineObject()->SetFrameTime(frame_time);
	actor_manager->GetPlayerObject()->GetCollision()->GetAffineObject()->SetFrameTime(frame_time);
}

HRESULT Scene::SceneManagerClass::UpdateActorCollisionState(ActorManagerClass* const& actor_manager, const InputClass* const& input)
{
	HRESULT result = S_OK;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT4 Default = DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f);
	DirectX::XMFLOAT4 OBB = DirectX::XMFLOAT4(1.f, 0.f, 0.f, 1.f);
	DirectX::XMFLOAT4 Ray = DirectX::XMFLOAT4(0.f, 0.f, 1.f, 1.f);

	// 에러 메세지, 변수 초기화 //
	e.title = _T("SceneManagerClass UpdateActorCollisionState()");

	// 충돌 처리
	for (int i = 0; i < actor_manager->GetOtherObjectCount(); ++i)
	{
		color = Default;

		if (actor_manager->GetPlayerObject()->GetCollision()->GetCollideState(actor_manager->GetOtherObject(i)->GetCollision()->GetCollision()) != DirectX::ContainmentType::DISJOINT)
			color = OBB;
		if (input->IsSpacebarPressed())
		{
			if (actor_manager->GetOtherObject(i)->GetCollision()->GetCollideState(actor_manager->GetPlayerObject()->GetRay()) == DirectX::ContainmentType::CONTAINS)
				color = Ray;
		}

		actor_manager->GetPlayerObject()->GetCollision()->SetColor(color);
		actor_manager->GetOtherObject(i)->GetCollision()->SetColor(color);
	}

	return result;
}

HRESULT Scene::SceneManagerClass::UpdateActorAffine(ActorManagerClass* const& actor_manager, const InputClass* const& input)
{
	HRESULT result = S_OK;
	bool KeyDown = false;
	long MouseX = 0, MouseY = 0;

	// 에러 메세지, 변수 초기화 //
	e.title = _T("SceneManagerClass UpdateActorCollisionState()");

	// 이동 처리 //
	// 앞
	KeyDown = input->IsWBottunPressed();
	result = actor_manager->GetPlayerObject()->GetAffineObject()->ChangePosition(MoveState::MOVE_FORWARD, KeyDown);
	if (FAILED(result))
	{
		e.contents = _T("player 객체의 transform 변경(FORWARD) 실패");
		e.errorCode = result;
		return result;
	}
	result = actor_manager->GetPlayerObject()->GetCollision()->GetAffineObject()->ChangePosition(MoveState::MOVE_FORWARD, KeyDown);
	if (FAILED(result))
	{
		e.contents = _T("player 객체의 collision의 transform 변경(FORWARD) 실패");
		e.errorCode = result;
		return result;
	}

	// 뒤
	KeyDown = input->IsSBottunPressed();
	result = actor_manager->GetPlayerObject()->GetAffineObject()->ChangePosition(MoveState::MOVE_BACKWARD, KeyDown);
	if (FAILED(result))
	{
		e.contents = _T("player 객체의 transform 변경(BACKWARD) 실패");
		e.errorCode = result;
		return result;
	}
	result = actor_manager->GetPlayerObject()->GetCollision()->GetAffineObject()->ChangePosition(MoveState::MOVE_BACKWARD, KeyDown);
	if (FAILED(result))
	{
		e.contents = _T("player 객체의 collision의 transform 변경(BACKWARD) 실패");
		e.errorCode = result;
		return result;
	}

	// 왼쪽
	KeyDown = input->IsABottunPressed();
	result = actor_manager->GetPlayerObject()->GetAffineObject()->ChangePosition(MoveState::MOVE_LEFT, KeyDown);
	if (FAILED(result))
	{
		e.contents = _T("player 객체의 transform 변경(LEFT) 실패");
		e.errorCode = result;
		return result;
	}
	result = actor_manager->GetPlayerObject()->GetCollision()->GetAffineObject()->ChangePosition(MoveState::MOVE_LEFT, KeyDown);
	if (FAILED(result))
	{
		e.contents = _T("player 객체의 collision의 transform 변경(LEFT) 실패");
		e.errorCode = result;
		return result;
	}

	// 오른쪽
	KeyDown = input->IsDBottunPressed();
	result = actor_manager->GetPlayerObject()->GetAffineObject()->ChangePosition(MoveState::MOVE_RIGHT, KeyDown);
	if (FAILED(result))
	{
		e.contents = _T("player 객체의 transform 변경(RIGHT) 실패");
		e.errorCode = result;
		return result;
	}
	result = actor_manager->GetPlayerObject()->GetCollision()->GetAffineObject()->ChangePosition(MoveState::MOVE_RIGHT, KeyDown);
	if (FAILED(result))
	{
		e.contents = _T("player 객체의 collision의 transform 변경(RIGHT) 실패");
		e.errorCode = result;
		return result;
	}

	// 회전
	KeyDown = input->IsMouseCenterBottunPressed();
	if (KeyDown)
	{
		input->GetMouseMoveDelta(MouseX, MouseY);
		actor_manager->GetPlayerObject()->GetAffineObject()->ChangeRotation(MouseX, MouseY);
		actor_manager->GetPlayerObject()->GetCollision()->GetAffineObject()->ChangeRotation(MouseX, MouseY);

		float x = actor_manager->GetPlayerObject()->GetAffineObject()->GetRotation().x;
		if (!(0.f <= x && x <= 0.15f))
		{
			DirectX::XMFLOAT4 rot = actor_manager->GetPlayerObject()->GetAffineObject()->GetRotation();
			if (x > 0.15f)
				rot.x = 0.15f;
			if (x < 0.f)
				rot.x = 0.f;
			actor_manager->GetPlayerObject()->GetAffineObject()->SetRotation(rot);
			actor_manager->GetPlayerObject()->GetCollision()->GetAffineObject()->SetRotation(rot);
		}

		if (m_IsShowingCursor)
		{
			ShowCursor(false);
			m_IsShowingCursor = false;
		}
	}
	else
	{
		if (!m_IsShowingCursor)
		{
			m_IsShowingCursor = true;
			ShowCursor(true);

			// 마우스 커서 위치를 중앙으로 초기화 //
			SetCursorPos(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
		}
	}

	return result;
}
