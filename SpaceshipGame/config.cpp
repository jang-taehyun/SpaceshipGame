#include "pch.h"
#include "config.h"

// System 관련 전역 변수 초기화 //
UINT System::WIDTH = 1200;
UINT System::HEIGHT = 800;

bool System::FULL_SCREEN = false;
bool System::VSYNC_ENABLED = true;
float System::SCREEN_DEPTH = 1000.0f;
float System::SCREEN_NEAR = 0.1f;

float System::MOUSE_SENSITIVITY = 0.0015f;

// Sound 관련 전역 변수 초기화 //
UINT Sound::SoundIDCount = 3;
std::map<Sound::ID, const std::wstring> Sound::SoundFileList =
{
	{ Sound::ID::BACKGROUND, _T("./data/dedede.wav") },
	{ Sound::ID::EFFECT, _T("./data/sound01.wav") }
};

// Model 관련 전역 변수 초기화 //
UINT Graphic::Model::ModelIDCount = 3;
std::map<Graphic::Model::ID, const std::wstring> Graphic::Model::ModelFileList =
{
			{ Graphic::Model::ID::COLLISION, _T("./data/cube.txt") },
			{ Graphic::Model::ID::DEFAULT_SPACESHIP, _T("./data/aircraft.txt") },
};

// UI texture 관련 전역 변수 초기화 //
UINT Graphic::Texture::UITextureIDCount = 3;
std::map<Graphic::Texture::UITextureID, const std::wstring> Graphic::Texture::UITextureFileList =
{
	{ Graphic::Texture::UITextureID::START_BACKGROUND, _T("./resource/StartBackground.jpg") },
	{ Graphic::Texture::UITextureID::START_BUTTON, _T("./resource/button.png") }
};

// Shader 관련 전역 변수 초기화 //
UINT Graphic::Shader::ShaderIDCount = 3;

// Font 관련 전역 변수 초기화 //
UINT Graphic::Font::FontIDCount = 2;
std::map<Graphic::Font::ID, const std::wstring> Graphic::Font::FontFileList =
{
	{ Graphic::Font::ID::DEFAULT, _T("./resource/굴림.spritefont") },
};