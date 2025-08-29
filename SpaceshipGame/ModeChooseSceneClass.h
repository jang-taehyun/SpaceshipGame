#pragma once

#include "SceneClass.h"

namespace System { class InputClass; }
namespace Object { class IObjectClass; }

namespace Scene
{
	class ModeChooseSceneClass : public SceneClass
	{
	public:
		ModeChooseSceneClass(ID next, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds);
		ModeChooseSceneClass(const ModeChooseSceneClass& other);
		ModeChooseSceneClass(ModeChooseSceneClass&& other) noexcept;
		virtual ~ModeChooseSceneClass() = default;

		ModeChooseSceneClass& operator=(const ModeChooseSceneClass& other);
		ModeChooseSceneClass& operator=(ModeChooseSceneClass&& other) noexcept;

		virtual void Frame(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time) override;
		virtual std::unique_ptr<ISceneClass> Clone() const override { return std::make_unique<ModeChooseSceneClass>(*this); }

	private:
		UINT m_SingleModeButtonIdx = 0;
		UINT m_MultiModeButtonIdx = 0;
	};
}
