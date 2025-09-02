#pragma once

#include "SceneClass.h"

// namespace System { class InputClass; }
// namespace Object { class IObjectClass; }

namespace Scene
{
	class StartSceneClass : public SceneClass
	{
	private:
		enum class ObjectID
		{
			POPUP_BACKGROUND,
			POPUP_UI,
			POPUP_UI_TEXT,
			POPUP_BUTTON_UI_OK,
			POPUP_BUTTON_UI_CANCEL,
			POPUP_BUTTON_TEXT_OK,
			POPUP_BUTTON_TEXT_CANCEL,

			SCENE_BUTTON_UI_START,
			SCENE_BUTTON_TEXT_START, 
			SCENE_BACKGROUND,
			SCENE_LOGO,
		};

	public:
		StartSceneClass(ID next, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds);
		StartSceneClass(const StartSceneClass& other);
		StartSceneClass(StartSceneClass&& other) noexcept;
		virtual ~StartSceneClass() = default;

		StartSceneClass& operator=(const StartSceneClass& other);
		StartSceneClass& operator=(StartSceneClass&& other) noexcept;

		virtual void Frame(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time) override;
		virtual std::unique_ptr<ISceneClass> Clone() const override { return std::make_unique<StartSceneClass>(*this); }

	private:
		void LoadLogo(UI::UIManagerClass* UIs);
		void LoadSound(Sound::SoundManagerClass* sounds);
		void Load2DBackground(UI::UIManagerClass* UIs);
		void LoadGameStartButton(Text::TextManagerClass* texts, UI::UIManagerClass* UIs);
		void LoadESCPopupWindow(Text::TextManagerClass* texts, UI::UIManagerClass* UIs);

		void ActiveESCPopup(Text::TextManagerClass* texts, UI::UIManagerClass* UIs);
		void DeactiveESCPopup(Text::TextManagerClass* texts, UI::UIManagerClass* UIs);

		bool ProcessStartButton(UI::IUIClass* button);
		bool ProcessESCPopUp(const System::InputClass* input, Text::TextManagerClass* texts, UI::UIManagerClass* UIs);

	private:
		std::map<ObjectID, UINT> m_ObjectList;
		bool m_ESCPopupActive = false;
	};
}