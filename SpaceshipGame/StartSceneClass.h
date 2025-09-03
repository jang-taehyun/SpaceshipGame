#pragma once

#include "SceneClass.h"

namespace Scene
{
	class StartSceneClass : public SceneClass
	{
	private:
		enum class ObjectID
		{
			SCENE_BACKGROUND,
			SCENE_LOGO,
			SCENE_BUTTON_UI_START,
			SCENE_BUTTON_TEXT_START, 
		};

	public:
		StartSceneClass(ID next, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds);
		StartSceneClass(const StartSceneClass& other);
		StartSceneClass(StartSceneClass&& other) noexcept;
		virtual ~StartSceneClass() = default;

		StartSceneClass& operator=(const StartSceneClass& other);
		StartSceneClass& operator=(StartSceneClass&& other) noexcept;

		virtual std::unique_ptr<ISceneClass> Clone() const override { return std::make_unique<StartSceneClass>(*this); }

	private:
		virtual bool ProcessChildScene(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time) override;
		virtual void ActiveChildSceneUI(Text::TextManagerClass* texts, UI::UIManagerClass* UIs) override;
		virtual void DeactiveChildSceneUI(Text::TextManagerClass* texts, UI::UIManagerClass* UIs) override;

	private:
		void LoadLogo(UI::UIManagerClass* UIs);
		void LoadSound(Sound::SoundManagerClass* sounds);
		void Load2DBackground(UI::UIManagerClass* UIs);
		void LoadGameStartButton(Text::TextManagerClass* texts, UI::UIManagerClass* UIs);

		bool ProcessStartButton(UI::IUIClass* button);

	private:
		std::map<ObjectID, UINT> m_ObjectList;
	};
}