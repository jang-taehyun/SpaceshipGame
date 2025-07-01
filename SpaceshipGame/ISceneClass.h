#pragma once

namespace System { class InputClass; }

namespace Scene
{
	class ISceneClass
	{
	public:
		virtual ~ISceneClass() = default;

		virtual inline SceneState GetCurrentSceneState() const = 0;
		virtual inline SceneState GetNextSceneState() const = 0;
		virtual inline bool IsSceneEnded() const = 0;

		virtual void Frame(const System::InputClass* input, float frame_time) = 0;
		virtual inline std::unique_ptr<ISceneClass> Clone() const = 0;

	protected:
		virtual inline void SetNextScene(SceneState next) = 0;
		virtual inline void SetSceneEnded() = 0;
	};
}