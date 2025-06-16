#pragma once

class ISceneClass
{
public:
	virtual ~ISceneClass() = default;

	virtual inline SceneState GetSceneState() const = 0;
	virtual inline std::unique_ptr<ISceneClass> Clone() const = 0;
	virtual inline bool IsSceneEnd() const = 0;

private:
	virtual inline void SetSceneEnded() = 0;
};

