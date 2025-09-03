#pragma once

#include "SceneClass.h"

namespace Scene
{
	class ModeChooseSceneClass : public SceneClass
	{
	private:
		enum class ObjectID
		{
			SCENE_BACKGROUND,
			SCENE_BUTTON_UI_SINGLEMODE,
			SCENE_BUTTON_TEXT_SINGLEMODE,
			SCENE_BUTTON_UI_MULTIMODE,
			SCENE_BUTTON_TEXT_MULTIMODE,
		};

	public:
		ModeChooseSceneClass(ID next, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds);
		ModeChooseSceneClass(const ModeChooseSceneClass& other);
		ModeChooseSceneClass(ModeChooseSceneClass&& other) noexcept;
		virtual ~ModeChooseSceneClass() = default;

		ModeChooseSceneClass& operator=(const ModeChooseSceneClass& other);
		ModeChooseSceneClass& operator=(ModeChooseSceneClass&& other) noexcept;

		virtual std::unique_ptr<ISceneClass> Clone() const override { return std::make_unique<ModeChooseSceneClass>(*this); }

	private:
		virtual void ActiveChildSceneUI(Text::TextManagerClass* texts, UI::UIManagerClass* UIs) override;
		virtual void DeactiveChildSceneUI(Text::TextManagerClass* texts, UI::UIManagerClass* UIs) override;
		virtual bool ProcessChildScene(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time) override;

	private:
		void LoadSound(Sound::SoundManagerClass* sounds);
		void Load2DBackground(UI::UIManagerClass* UIs);
		void LoadSingleModeButton(Text::TextManagerClass* texts, UI::UIManagerClass* UIs);
		void LoadMultiModeButton(Text::TextManagerClass* texts, UI::UIManagerClass* UIs);

	private:
		bool ProcessSingleModeButton(UI::IUIClass* button);
		bool ProcessMultiModeButton(UI::IUIClass* button);

	private:
		std::map<ObjectID, UINT> m_ObjectList;
	};
}
