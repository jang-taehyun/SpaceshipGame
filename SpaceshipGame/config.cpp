#include "pch.h"
#include "config.h"

UINT System::RESOLUTION_COUNT;
UINT System::CURRENT_RESOLUTION_INDEX;
System::RESOLUTION System::RESOLUTIONS[2];

bool System::FULL_SCREEN;
bool System::VSYNC_ENABLED;
float System::SCREEN_DEPTH;
float System::SCREEN_NEAR;

float System::MOUSE_SENSITIVITY;

bool System::RUNNING = true;
HWND System::hwnd;
HINSTANCE System::hinst;
std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> System::SceneMessageHander = nullptr;

const float UI::ExtendSize = 1.3f;