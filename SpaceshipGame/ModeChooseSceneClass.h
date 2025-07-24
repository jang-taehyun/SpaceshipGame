#pragma once

#include "SceneClass.h"

namespace System { class InputClass; }
namespace Object { class IObjectClass; }

namespace Scene
{
	class ModeChooseSceneClass : public SceneClass
	{
	public:
		ModeChooseSceneClass(SceneID next, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds);
		ModeChooseSceneClass(const ModeChooseSceneClass& other);
		ModeChooseSceneClass(ModeChooseSceneClass&& other) noexcept;

		ModeChooseSceneClass& operator=(const ModeChooseSceneClass& other);
		ModeChooseSceneClass& operator=(ModeChooseSceneClass&& other) noexcept;

		virtual void Frame(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time) override;
		virtual inline Object::IObjectClass* GetActiveCamera() override { return m_Camera.get(); }
		virtual inline std::unique_ptr<ISceneClass> Clone() const override { return std::make_unique<ModeChooseSceneClass>(*this); }

	private:
		std::unique_ptr<Object::IObjectClass> m_Camera = nullptr;
	};
}
