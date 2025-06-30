#pragma once

#include "SceneClass.h"

namespace System { class InputClass; }

namespace Scene
{
	class StartSceneClass : public SceneClass
	{
	public:
		StartSceneClass(SceneState current, SceneState next);
		virtual ~StartSceneClass() = default;

		virtual void Frame(const System::InputClass* input, float frame_time) override;
		virtual inline std::unique_ptr<ISceneClass> Clone() const override { return std::make_unique<StartSceneClass>(*this); }
	};
}