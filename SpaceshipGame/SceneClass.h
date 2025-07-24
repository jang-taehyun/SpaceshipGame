#pragma once

#include "ISceneClass.h"

namespace Scene
{
	class SceneClass : public ISceneClass
	{
	public:
		SceneClass(SceneID current, SceneID next);
		virtual ~SceneClass();

		virtual inline SceneID GetCurrentSceneState() const override { return m_CurrentSceneState; }
		virtual inline SceneID GetNextSceneState() const override { return m_NextSceneState; }
		virtual inline bool IsSceneEnded() const override { return m_IsSceneEnded; }

		virtual void Frame(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time) override = 0;
		virtual inline Object::IObjectClass* GetActiveCamera() override = 0;
		virtual inline std::unique_ptr<ISceneClass> Clone() const override = 0;

	protected:
		virtual inline void SetSceneEnded() override { m_IsSceneEnded = true; }
		virtual inline void SetNextScene(SceneID next) override { m_NextSceneState = next; }

	private:
		SceneID m_CurrentSceneState = SceneID::NONE;
		SceneID m_NextSceneState = SceneID::NONE;
		bool m_IsSceneEnded = false;
		bool m_IsShowingCursor = true;
	};
}