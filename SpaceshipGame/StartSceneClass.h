#pragma once

#include "SceneClass.h"

namespace System { class InputClass; }

namespace Scene
{
	class StartSceneClass : public SceneClass
	{
	public:
		StartSceneClass(SceneState current, Object::ObjectManagerClass* actors, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds);
		virtual ~StartSceneClass() = default;

		virtual void Frame(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time) override;
		virtual inline std::unique_ptr<ISceneClass> Clone() const override { return std::make_unique<StartSceneClass>(*this); }
	};
}