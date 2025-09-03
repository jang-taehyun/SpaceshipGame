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
		};

	public:
		SingleModeSceneClass(ID next, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds);
		SingleModeSceneClass(const SingleModeSceneClass& other);
		SingleModeSceneClass(SingleModeSceneClass&& other) noexcept;
		virtual ~SingleModeSceneClass() = default;

		SingleModeSceneClass& operator=(const SingleModeSceneClass& other);
		SingleModeSceneClass& operator=(SingleModeSceneClass&& other) noexcept;

		virtual Object::IObjectClass* GetActiveCamera() override { return m_Camera.get(); }
		virtual std::unique_ptr<ISceneClass> Clone() const override { return std::make_unique<SingleModeSceneClass>(*this); }

	private:
		virtual void LoadESCPopupWindow(Text::TextManagerClass* texts, UI::UIManagerClass* UIs) override;
		virtual bool ProcessESCPopUp(const System::InputClass* input, Text::TextManagerClass* texts, UI::UIManagerClass* UIs) override;
		virtual void ActiveESCPopup(Text::TextManagerClass* texts, UI::UIManagerClass* UIs) override;
		virtual void DeactiveESCPopup(Text::TextManagerClass* texts, UI::UIManagerClass* UIs) override;

		virtual void ActiveChildSceneUI(Text::TextManagerClass* texts, UI::UIManagerClass* UIs) override;
		virtual void DeactiveChildSceneUI(Text::TextManagerClass* texts, UI::UIManagerClass* UIs) override;
		virtual bool ProcessChildScene(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time) override;

	private:
		void LoadSound(Sound::SoundManagerClass* sounds);
		void LoadCamera();
		void LoadTerrain();
		void LoadObject(Object::ObjectManagerClass* objects);

		bool ProcessCamera(const System::InputClass* input, float frame_time);

	private:
		std::unique_ptr<Object::IObjectClass> m_Camera = nullptr;
		std::map<ObjectID, UINT> m_ObjectList;
	};
};