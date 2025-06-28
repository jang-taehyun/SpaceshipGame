#pragma once

#include <map>
#include "typedef.h"

namespace System
{
	// 해상도 //
	const int WIDTH = 1200;
	const int HEIGHT = 800;

	// Graphics 설정 //
	const bool FULL_SCREEN = false;
	const bool VSYNC_ENABLED = true;
	const float SCREEN_DEPTH = 1000.0f;
	const float SCREEN_NEAR = 0.1f;

	// MOUSE 감도 설정 //
	float MOUSE_SENSITIVITY = 0.0015f;
}

// object 관련
namespace Object
{
	// 최대 object의 개수 //
	const int OBJECT_MAX_COUNT = 6;

	// 이동 상태 정보 //
	enum class MoveState
	{
		MOVE_FORWARD, MOVE_BACKWARD, MOVE_RIGHT, MOVE_LEFT,
	};

	// collision 상태 정보 //
	enum class CollisionState
	{
		NONE = 0, OBB_HIT = 1, RAY_HIT = 2,
	};
}


// scene 관련 //
namespace Scene
{
	enum class SceneState
	{
		NONE = 0, START = 1, LOADING = 2, LOGIN, MATCH, INGAME,
	};
}


// UI 관련 //
namespace UI
{
	// UI의 ID //
	enum class ID
	{
		NONE = 0, START_BACKGROUND = 1, START_BUTTON = 2, LOADING_BACKGROUND = 4,
	};
}


// Text 관련 //
namespace Text
{
	enum class TextFlag
	{
		DEFAULT = 1,
	};

	// font 파일 정보 //
	const std::wstring FontFileName = _T("./resource/굴림.spritefont");
}


// sound 관련 //
namespace Sound
{
	enum class ID
	{
		BACKGROUND, EFFECT,
	};

	const std::map<ID, const std::wstring> SoundList =
	{
		{ ID::BACKGROUND, _T("./data/dedede.wav") },
		{ ID::EFFECT, _T("./data/sound01.wav") }
	};
}

// Graphic 관련 //
namespace Graphic
{
	// 3D model 관련 //
	namespace Model
	{
		enum class ID
		{
			DEFAULT_SPACESHIP,
			COLLISION,
		};

		const std::map<ID, const std::wstring> ModelList = {
			{ ID::COLLISION, _T("./data/cube.txt") },
			{ ID::DEFAULT_SPACESHIP, _T("./data/aircraft.txt") },
		};

		// PTN vertex type //
		struct PTN_VertexType
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT2 texture;
			DirectX::XMFLOAT3 normal;
		};
	}


	namespace Texture
	{
		// UI의 texture ID //
		enum class UITextureID
		{
			NONE = 0, START_BACKGROUND = 1, START_BUTTON = 2, LOADING_BACKGROUND = 4,
		};

		// UI texture의 파일 정보 //
		const std::map<UITextureID, const std::wstring> UITextureList =
		{
			{ UITextureID::START_BACKGROUND, _T("./resource/StartBackground.jpg") },
			{ UITextureID::START_BUTTON, _T("./resource/button.png") }
		};
	}


	namespace Shader
	{
		enum class ID
		{
			DEFAULT, ALPHA_MAP, TEXTURE, MULTI_TEXTURE, LIGHT, LIGHT_MAP, SPACESHIP, CUBE,
		};

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

		const std::map<ID, ShaderFileInfo> ShaderList = {
			{ ID::ALPHA_MAP, AlphaMapShaderInfo },
			{ ID::TEXTURE, TextureShaderInfo },
			{ ID::MULTI_TEXTURE, MultiTextureShaderInfo },
			{ ID::LIGHT, LightShaderInfo },
			{ ID::LIGHT_MAP, LightMapShaderInfo },
			{ ID::SPACESHIP, SpaceshipShaderInfo },
			{ ID::CUBE, CubeShaderInfo },
		};
	}
}