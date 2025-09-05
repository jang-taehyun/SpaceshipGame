#pragma once

#include "pch.h"
#include "config.h"

UINT System::RESOLUTION_COUNT;
UINT System::CURRENT_RESOLUTION_INDEX;
System::RESOLUTION System::RESOLUTIONS[2];

bool System::FULL_SCREEN;
bool System::VSYNC_ENABLED;
float System::SCREEN_DEPTH;
float System::SCREEN_NEAR;

float System::INTERPOLATION_MOUSE_SPEED;

bool System::RUNNING = true;
HWND System::hWnd;
HINSTANCE System::hInst;

const float UI::ExtendSize = 1.1f;