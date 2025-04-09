#pragma once
#include "typedef.h"

// 해상도 //
const int WIDTH = 1200;
const int HEIGHT = 800;

// position 상태 정보 //
enum class MoveState
{
	MOVE_FORWARD, MOVE_BACKWARD, MOVE_RIGHT, MOVE_LEFT,
};

// rotation 상태 정보 //
enum class RotationState
{
	ROTATE_UP, ROTATE_DOWN, ROTATE_RIGHT, ROTATE_LEFT,
};

// sound 파일 정보 //
const SoundFileInfo s_info = { _T("./data/sound01.wav") };

// font 파일 정보 //
const tstring FontFileName = _T("./data/font01.txt");
const tstring FontTextureFileName = _T("./data/font01.tga");

// 3D object 파일 정보 //
const tstring CubeModelFileName = _T("./data/aircraft.txt");
const std::vector<tstring> CubeTextureFileNames =
{
	_T("./data/aircraft.jpg")
};

// 3D object 모델 정보 //
const int NUMBER_OF_ALL_MODELS = 1;
enum class ModelIDs
{
	DEFAULT_SPACESHIP, 
};

// shader 관련 정보 //
const ShaderFileInfo AlphaMapShaderInfo =
{
	_T("./shader/vertex/alphamap.vs"),
	_T("./shader/pixel/alphamap.ps"),
	_T("AlphaMapVertexShader"),
	_T("AlphaMapPixelShader")
};

const ShaderFileInfo TextureShaderInfo =
{
	_T("./shader/vertex/texture.vs"),
	_T("./shader/pixel/texture.ps"),
	_T("TextureVertexShader"),
	_T("TexturePixelShader")
};

const ShaderFileInfo MultiTextureShaderInfo =
{
	_T("./shader/vertex/multitexture.vs"),
	_T("./shader/pixel/multitexture.ps"),
	_T("MultiTextureVertexShader"),
	_T("MultiTexturePixelShader")
};

const ShaderFileInfo LightShaderInfo =
{
	_T("./shader/vertex/light.vs"),
	_T("./shader/pixel/light.ps"),
	_T("LightVertexShader"),
	_T("LightPixelShader")
};

const ShaderFileInfo LightMapShaderInfo =
{
	_T("./shader/vertex/lightmap.vs"),
	_T("./shader/pixel/lightmap.ps"),
	_T("LightMapVertexShader"),
	_T("LightMapPixelShader")
};

const ShaderFileInfo FontShaderInfo =
{
	_T("./shader/vertex/font.vs"),
	_T("./shader/pixel/font.ps"),
	_T("FontVertexShader"),
	_T("FontPixelShader")
};