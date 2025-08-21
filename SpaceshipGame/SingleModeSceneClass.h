#pragma once

#include "SceneClass.h"

namespace Scene
{
	class SingleModeSceneClass : public SceneClass
	{
	public:
		SingleModeSceneClass(ID next, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds);
		SingleModeSceneClass(const SingleModeSceneClass& other);
		SingleModeSceneClass(SingleModeSceneClass&& other) noexcept;
		virtual ~SingleModeSceneClass() = default;

		SingleModeSceneClass& operator=(const SingleModeSceneClass& other);
		SingleModeSceneClass& operator=(SingleModeSceneClass&& other) noexcept;

		virtual void Frame(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time) override;
		virtual Object::IObjectClass* GetActiveCamera() override { return m_Camera.get(); }
		virtual std::unique_ptr<ISceneClass> Clone() const override { return std::make_unique<SingleModeSceneClass>(*this); }

	private:
		std::unique_ptr<Object::IObjectClass> m_Camera = nullptr;
	};
};


