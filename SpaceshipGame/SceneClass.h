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

		virtual inline UINT GetModelIDs() const override { return m_ModelIDs; }
		virtual inline UINT GetUITextureIDs() const override { return m_UITextureIDs; }

		virtual inline Object::IObjectClass* GetCamera() const override { return m_Camera.get(); }

		virtual void Frame(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time) override = 0;
		virtual inline std::unique_ptr<ISceneClass> Clone() const override = 0;

	protected:
		virtual inline void SetSceneEnded() override { m_IsSceneEnded = true; }

		virtual inline void SetNextScene(SceneState next) override { m_NextSceneState = next; }
		virtual inline void SetModelIDs(UINT IDs) override { m_ModelIDs = IDs; }
		virtual inline void SetUITextureIDs(UINT IDs) override { m_UITextureIDs = IDs; }

	private:
		SceneState m_CurrentSceneState = SceneState::NONE;
		SceneState m_NextSceneState = SceneState::NONE;
		bool m_IsSceneEnded = false;
		bool m_IsShowingCursor = true;

		UINT m_ModelIDs = 0;
		UINT m_UITextureIDs = 0;

		std::unique_ptr<Object::IObjectClass> m_Camera = nullptr;
	};
}