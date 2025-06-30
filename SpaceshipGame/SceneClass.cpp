#include "pch.h"
#include "SceneClass.h"

Scene::SceneClass::SceneClass(SceneState current, SceneState next) : m_CurrentSceneState(current), m_NextSceneState(next) {}
