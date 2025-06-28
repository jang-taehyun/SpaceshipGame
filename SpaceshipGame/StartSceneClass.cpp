#include "pch.h"
#include "InputClass.h"
#include "StartSceneClass.h"

void Scene::StartSceneClass::Frame(const System::InputClass* input)
{
	if (input->IsSpacebarPressed())
		SetSceneEnded();
}
