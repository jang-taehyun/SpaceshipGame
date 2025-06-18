#pragma once

#include "ISceneClass.h"

class BaseSceneClass : public ISceneClass
{
public:
	virtual ~BaseSceneClass() = default;

	virtual inline SceneState GetSceneState() const override { return m_SceneState; }
	virtual inline bool IsSceneEnded() const override { return m_IsSceneEnded; }

protected:
	virtual inline void SetSceneEnded() override { m_IsSceneEnded = true; }

private:
	SceneState m_SceneState = SceneState::NONE;
	bool m_IsSceneEnded = false;
};

