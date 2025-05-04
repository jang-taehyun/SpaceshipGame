#pragma once

class InputClass;
class CameraClass;
class ActorManagerClass;

class ProcessManagerClass
{
public:
	explicit ProcessManagerClass();
	virtual ~ProcessManagerClass();

public:
	HRESULT Frame(ActorManagerClass* const& actor_manager, CameraClass* const& camera, const InputClass* const& input, const float& frame_time);

private:
	HRESULT ProcessCamera(CameraClass* const& camera, const InputClass* const& input, const float& frame_time);
	HRESULT ProcessActor(ActorManagerClass* const& actor_manager, const InputClass* const& input);

private:
	void UpdateCameraFrameTime(CameraClass* const& camera, const float& frame_time);
	HRESULT UpdateCameraAffine(CameraClass* const& camera, const InputClass* const& input);
	HRESULT UpdateActorCollisionState(ActorManagerClass* const& actor_manager, const InputClass* const& input);

private:
	static bool IsInitialize;
	bool m_IsShowingCursor = true;

public:
	ProcessManagerClass(const ProcessManagerClass& other) = delete;
};

