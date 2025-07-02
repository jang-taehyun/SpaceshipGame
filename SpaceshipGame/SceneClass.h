#pragma once

#include "ISceneClass.h"

namespace Scene
{
	class SceneClass : public ISceneClass
	{
	public:
		SceneClass(SceneState current);
		virtual ~SceneClass();

		virtual inline SceneState GetCurrentSceneState() const override { return m_CurrentSceneState; }
		virtual inline SceneState GetNextSceneState() const override { return m_NextSceneState; }
		virtual inline bool IsSceneEnded() const override { return m_IsSceneEnded; }

		virtual void Frame(const System::InputClass* input, float frame_time) override = 0;
		virtual inline std::unique_ptr<ISceneClass> Clone() const override = 0;

		virtual inline Text::TextManagerClass* GetTextManager() const override { return m_TextManager.get(); }
		virtual inline UI::UIManagerClass* GetUIManager() const override { return m_UIManager.get(); }
		virtual inline Object::ActorManagerClass* GetActorManager() const override { return m_ActorManager.get(); }

		virtual inline Object::IObjectClass* GetCamera() const override { return m_Camera.get(); }

	protected:
		virtual inline Sound::SoundManagerClass* GetSoundManager() const override { return m_SoundManager.get(); }
		virtual inline void SetNextScene(SceneState next) override { m_NextSceneState = next; }
		virtual inline void SetSceneEnded() override { m_IsSceneEnded = true; }

	private:
		SceneState m_CurrentSceneState = SceneState::NONE;
		SceneState m_NextSceneState = SceneState::NONE;
		bool m_IsSceneEnded = false;
		bool m_IsShowingCursor = true;

		std::unique_ptr<Text::TextManagerClass> m_TextManager = nullptr;
		std::unique_ptr<UI::UIManagerClass> m_UIManager = nullptr;
		std::unique_ptr<Sound::SoundManagerClass> m_SoundManager = nullptr;
		std::unique_ptr<Object::ActorManagerClass> m_ActorManager = nullptr;
		std::unique_ptr<Object::IObjectClass> m_Camera = nullptr;
	};
}