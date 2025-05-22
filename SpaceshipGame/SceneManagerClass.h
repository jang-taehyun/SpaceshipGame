#pragma once

class InputClass;
class CameraClass;
class ActorManagerClass;

class SceneManagerClass
{
public:
	SceneManagerClass();
	virtual ~SceneManagerClass();

public:
	HRESULT Frame(ActorManagerClass* const& actor_manager, CameraClass* const& camera, const InputClass* const& input, const float& frame_time);
	const std::wstring& GetSceneString() const { return m_SceneString; }

private:
	HRESULT ProcessCamera(CameraClass* const& camera, const InputClass* const& input, const float& frame_time);
	HRESULT ProcessActor(ActorManagerClass* const& actor_manager, const InputClass* const& input, const float& frame_time);
	void ProcessSceneInfo();

private:
	void UpdateCameraFrameTime(CameraClass* const& camera, const float& frame_time);
	HRESULT UpdateCameraAffine(CameraClass* const& camera, const InputClass* const& input);
	void UpdateObjectFrameTime(ActorManagerClass* const& actor_manager, const float& frame_time);
	HRESULT UpdateActorCollisionState(ActorManagerClass* const& actor_manager, const InputClass* const& input);
	HRESULT UpdateActorAffine(ActorManagerClass* const& actor_manager, const InputClass* const& input);

private:
	static bool IsInitialize;
	bool m_IsShowingCursor = true;
	std::wstring m_SceneString = _T("");
	SceneState m_SceneState = SceneState::NONE;

public:
	SceneManagerClass(const SceneManagerClass& other) = delete;
};

