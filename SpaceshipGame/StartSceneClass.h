#pragma once

#include "BaseSceneClass.h"

namespace System { class InputClass; }

namespace Scene
{
	class StartSceneClass : public BaseSceneClass
	{
	public:
		StartSceneClass() = default;
		virtual ~StartSceneClass() = default;

		virtual inline std::unique_ptr<ISceneClass> Clone() const override { return std::make_unique<StartSceneClass>(*this); }

		virtual void Frame(const System::InputClass* input);

	private:
		SceneState m_CurrentSceneState = SceneState::START;
		SceneState m_NextSceneState = SceneState::LOGIN;
	};

}