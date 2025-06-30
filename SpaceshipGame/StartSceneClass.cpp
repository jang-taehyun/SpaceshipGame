#include "pch.h"
#include "InputClass.h"
#include "StartSceneClass.h"

Scene::StartSceneClass::StartSceneClass(SceneState current, SceneState next) : SceneClass(current, next) {}

void Scene::StartSceneClass::Frame(const System::InputClass* input, float frame_time)
{
	if (input->IsSpacebarPressed())
		SetSceneEnded();
}
