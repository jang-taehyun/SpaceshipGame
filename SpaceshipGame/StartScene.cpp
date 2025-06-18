#include "pch.h"
#include "InputClass.h"
#include "StartScene.h"

void StartScene::Frame(const InputClass* input)
{
	if (input->IsSpacebarPressed())
		SetSceneEnded();
}
