#pragma once

#include "SceneClass.h"

namespace System { class InputClass; }
namespace Object { class IObjectClass; }

namespace Scene
{
	class StartSceneClass : public SceneClass
	{
	private:
		enum class ObjectID
		{
			UI_SCENE_BACKGROUND,
			UI_START_BUTTON,
			TEXT_START_STRING,
			UI_POPUP_BACKGROUND,
		};

	public:
		StartSceneClass(ID next, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds);
		StartSceneClass(const StartSceneClass& other);
		StartSceneClass(StartSceneClass&& other) noexcept;
		virtual ~StartSceneClass();

		StartSceneClass& operator=(const StartSceneClass& other);
		StartSceneClass& operator=(StartSceneClass&& other) noexcept;

		LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

		virtual void Frame(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time) override;
		virtual std::unique_ptr<ISceneClass> Clone() const override { return std::make_unique<StartSceneClass>(*this); }

	private:
		std::map<ObjectID, UINT> m_ObjectList;
		HWND m_hStartButton = 0;
	};
}