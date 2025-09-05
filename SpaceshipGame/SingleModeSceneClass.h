#pragma once

#include "SceneClass.h"

namespace Scene
{
	class SingleModeSceneClass : public SceneClass
	{
	private:
		enum class ObjectID
		{
			POPUP_BUTTON_UI_PREVIOUS,
			POPUP_BUTTON_TEXT_PREVIOUS,

			SCENE_TEXT_SCORE_TITLE,
			SCENE_TEXT_SCORE,

			SCENE_ACTOR_SPACESHIP,
		};

	public:
		SingleModeSceneClass(ID next, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds);
		SingleModeSceneClass(const SingleModeSceneClass& other);
		SingleModeSceneClass(SingleModeSceneClass&& other) noexcept;
		virtual ~SingleModeSceneClass();

		SingleModeSceneClass& operator=(const SingleModeSceneClass& other);
		SingleModeSceneClass& operator=(SingleModeSceneClass&& other) noexcept;

		virtual std::unique_ptr<ISceneClass> Clone() const override { return std::make_unique<SingleModeSceneClass>(*this); }

	private:
		virtual void LoadESCPopupWindow(Text::TextManagerClass* texts, UI::UIManagerClass* UIs) override;
		virtual bool ProcessESCPopUp(const System::InputClass* input, Text::TextManagerClass* texts, UI::UIManagerClass* UIs) override;
		virtual void ActiveESCPopup(Text::TextManagerClass* texts, UI::UIManagerClass* UIs) override;
		virtual void DeactiveESCPopup(Text::TextManagerClass* texts, UI::UIManagerClass* UIs) override;

		virtual void ActiveChildSceneUI(Text::TextManagerClass* texts, UI::UIManagerClass* UIs) override;
		virtual void DeactiveChildSceneUI(Text::TextManagerClass* texts, UI::UIManagerClass* UIs) override;
		virtual void ProcessChildScene(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time) override;

	private:
		void LoadSound(Sound::SoundManagerClass* sounds);
		void LoadTerrain();
		void LoadScoreText(Text::TextManagerClass* texts);
		void LoadObject(Object::ObjectManagerClass* objects);

		void LimitMouseCursorMoveRange();

	private:
		std::map<ObjectID, UINT> m_UIIdxList;
		std::map<ObjectID, UINT> m_ObjectIdxList;

		RECT m_OriginMouseCursorMoveRange = {};
		RECT m_CurrentMouseCursorMoveRange = {};
	};
};