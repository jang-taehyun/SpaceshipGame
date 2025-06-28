#pragma once

namespace Scene
{
	class ISceneClass
	{
	public:
		virtual ~ISceneClass() = default;

		virtual inline SceneState GetSceneState() const = 0;
		virtual inline bool IsSceneEnded() const = 0;

		virtual inline std::unique_ptr<ISceneClass> Clone() const = 0;

	protected:
		virtual inline void SetSceneEnded() = 0;
	};
}