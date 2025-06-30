#pragma once

#include "ISceneClass.h"

namespace Scene
{
	class SceneClass : public ISceneClass
	{
	public:
		SceneClass(SceneState current, SceneState next);
		virtual ~SceneClass() = default;

		virtual inline SceneState GetCurrentSceneState() const override { return m_CurrentSceneState; }
		virtual inline SceneState GetNextSceneState() const override { return m_NextSceneState; }
		virtual inline bool IsSceneEnded() const override { return m_IsSceneEnded; }

		virtual void Frame(const System::InputClass* input, float frame_time) override = 0;
		virtual inline std::unique_ptr<ISceneClass> Clone() const override = 0;

	protected:
		virtual inline void SetSceneEnded() override { m_IsSceneEnded = true; }

	private:
		SceneState m_CurrentSceneState = SceneState::NONE;
		SceneState m_NextSceneState = SceneState::NONE;
		bool m_IsSceneEnded = false;
	};
}