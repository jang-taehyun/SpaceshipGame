#pragma once

#include "SceneClass.h"

namespace System { class InputClass; }
namespace Object { class IObjectClass; }

namespace Scene
{
	class StartSceneClass : public SceneClass
	{
	public:
		StartSceneClass(SceneID next, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds);
		StartSceneClass(const StartSceneClass& other);
		StartSceneClass(StartSceneClass&& other) noexcept;
		virtual ~StartSceneClass() = default;

		StartSceneClass& operator=(const StartSceneClass& other);
		StartSceneClass& operator=(StartSceneClass&& other) noexcept;

		virtual void Frame(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time) override;
		virtual Object::IObjectClass* GetActiveCamera() override { return m_Camera.get(); }
		virtual std::unique_ptr<ISceneClass> Clone() const override { return std::make_unique<StartSceneClass>(*this); }

	private:
		std::unique_ptr<Object::IObjectClass> m_Camera = nullptr;
	};
}