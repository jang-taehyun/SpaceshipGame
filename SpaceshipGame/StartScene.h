#pragma once

#include "ISceneClass.h"

class InputClass;

class StartScene : public ISceneClass
{
public:
	StartScene() = default;
	virtual ~StartScene() = default;

	virtual bool Frame(const InputClass* input);

	virtual inline SceneState GetSceneState() const override { return m_SceneState; }
	virtual inline std::unique_ptr<ISceneClass> Clone() const override { return std::make_unique<StartScene>(*this); }

	virtual inline bool IsSceneEnd() const override { return m_IsSceneEnded; }

private:
	virtual inline void SetSceneEnded() override { m_IsSceneEnded = true; }

private:
	SceneState m_SceneState = SceneState::START;
	bool m_IsSceneEnded = false;
};

