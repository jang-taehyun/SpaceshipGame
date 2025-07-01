#pragma once

#include <functional>

namespace Scene { class ISceneClass; }

namespace Scene
{
	std::unique_ptr<ISceneClass> CreateStartScene(SceneState cur, SceneState next);

	class SceneLoaderClass
	{
	public:
		SceneLoaderClass();
		virtual ~SceneLoaderClass();

		std::unique_ptr<ISceneClass> CreateScene(SceneState cur, SceneState next);

	private:
		static bool IsInitailize;

		std::map<SceneState, std::function<std::unique_ptr<ISceneClass>(SceneState, SceneState)>> m_Creator;
	};
}
