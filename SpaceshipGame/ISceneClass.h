#pragma once

class TextManagerClass;
class ActorManagerClass;

class ISceneClass
{
public:
	virtual ~ISceneClass() = default;

	virtual HRESULT Frame(const TextManagerClass* TextManager, const ActorManagerClass* ActorManager, float FrameTime) = 0;

	virtual SceneState GetSceneState() const = 0;

	virtual std::unique_ptr<ISceneClass> Clone() const = 0;
};

