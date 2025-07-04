#include "pch.h"
#include "ObjectManagerClass.h"
#include "TextManagerClass.h"
#include "UIManagerClass.h"
#include "SoundManagerClass.h"
#include "InputClass.h"
#include "StartSceneClass.h"

Scene::StartSceneClass::StartSceneClass(SceneState current, Object::ObjectManagerClass* actors, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds) : SceneClass(current)
{
	
}

void Scene::StartSceneClass::Frame(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time)
{
	if (input->IsSpacebarPressed())
		SetSceneEnded();
}
