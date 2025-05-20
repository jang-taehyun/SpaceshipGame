#pragma once
#include "typedef.h"

// 해상도 //
const int WIDTH = 1200;
const int HEIGHT = 800;

// Graphics 설정 //
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

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

// collision 상태 정보 //
enum class CollisionState
{
	NONE = 0, OBB_HIT = 1, RAY_HIT = 2,
};

// scene 정보 //
enum class SceneState
{
	NONE = 0, LOADING = 1, MATCH = 2, INGAME = 3,
};

// sound 파일 정보 //
enum class SoundInfo
{
	BACKGROUND, EFFECT, 
};
const SoundFileInfo BackgroundSoundInfo = { _T("./data/dedede.wav") };
const SoundFileInfo EffectSoundInfo = { _T("./data/sound01.wav") };

// font 파일 정보 //
const std::wstring FontFileName = _T("./resource/굴림.spritefont");

// 3D object 파일 정보 //
const std::wstring SpaceModelFileName = _T("./data/aircraft.txt");
const std::vector<std::wstring> SpaceTextureFileNames =
{
	_T("./data/aircraft.jpg")
};


const std::wstring CubeModelFileName = _T("./data/cube.txt");
const std::vector<std::wstring> CubeTextureFileNames =
{
	_T("./data/stone01.tga"),
	_T("./data/dirt01.dds"),
};

// 3D object 모델 관련 flag //
const int NUMBER_OF_ALL_MODELS = 2;
enum class ModelIDs
{
	DEFAULT_SPACESHIP,
	DEFAULT_CUBE,
};

// shader 관련 정보 //
const ShaderFileInfo AlphaMapShaderInfo =
{
	_T("./shader/vertex/alphamap.vs"),
	_T("./shader/pixel/alphamap.ps"),
	"AlphaMapVertexShader",
	"AlphaMapPixelShader"
};

const ShaderFileInfo TextureShaderInfo =
{
	_T("./shader/vertex/texture.vs"),
	_T("./shader/pixel/texture.ps"),
	"TextureVertexShader",
	"TexturePixelShader"
};

const ShaderFileInfo MultiTextureShaderInfo =
{
	_T("./shader/vertex/multitexture.vs"),
	_T("./shader/pixel/multitexture.ps"),
	"MultiTextureVertexShader",
	"MultiTexturePixelShader"
};

const ShaderFileInfo LightShaderInfo =
{
	_T("./shader/vertex/light.vs"),
	_T("./shader/pixel/light.ps"),
	"LightVertexShader",
	"LightPixelShader"
};

const ShaderFileInfo LightMapShaderInfo =
{
	_T("./shader/vertex/lightmap.vs"),
	_T("./shader/pixel/lightmap.ps"),
	"LightMapVertexShader",
	"LightMapPixelShader"
};

const ShaderFileInfo FontShaderInfo =
{
	_T("./shader/vertex/font.vs"),
	_T("./shader/pixel/font.ps"),
	"FontVertexShader",
	"FontPixelShader"
};

const ShaderFileInfo SpaceshipShaderInfo =
{
	_T("./shader/vertex/Spaceship.vs"),
	_T("./shader/pixel/Spaceship.ps"),
	"SpaceshipVertexShader",
	"SpaceshipPixelShader"
};

const ShaderFileInfo CubeShaderInfo =
{
	_T("./shader/vertex/color.vs"),
	_T("./shader/pixel/color.ps"),
	"ColorVertexShader",
	"ColorPixelShader"
};

// 3D model 관련 정보들 //
const ModelInfo SpaceshipModelInfo =
{
	SpaceModelFileName,
	SpaceTextureFileNames,
	SpaceshipShaderInfo
};

const ModelInfo CubeModelInfo =
{
	CubeModelFileName,
	CubeTextureFileNames,
	CubeShaderInfo
};