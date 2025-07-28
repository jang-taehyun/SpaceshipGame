#include "pch.h"
#include "config.h"

UINT System::RESOLUTION_COUNT;
UINT System::CURRENT_RESOLUTION_INDEX;
std::vector<System::RESOLUTION> System::RESOLUTIONS;

bool System::FULL_SCREEN;
bool System::VSYNC_ENABLED;
float System::SCREEN_DEPTH;
float System::SCREEN_NEAR;

float System::MOUSE_SENSITIVITY;

UINT Sound::SoundIDCount;
std::map<Sound::ID, const std::wstring> Sound::SoundFileList;

UINT Graphic::Model::ModelIDCount;
std::map<Graphic::Model::ID, const std::string> Graphic::Model::ModelFileList;
std::map<Graphic::Model::ID, const std::wstring> Graphic::Model::ModelTexturePathList;

// UI texture 관련 전역 변수 초기화 //
UINT Graphic::Texture::UITextureIDCount;
std::map<Graphic::Texture::UITextureID, const std::wstring> Graphic::Texture::UITextureFileList;

// Shader 관련 전역 변수 초기화 //
UINT Graphic::Shader::ShaderIDCount;

// Font 관련 전역 변수 초기화 //
UINT Graphic::Font::FontIDCount;
std::map<Graphic::Font::ID, const std::wstring> Graphic::Font::FontFileList;