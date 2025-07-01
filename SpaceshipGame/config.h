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
		NONE, OBB_HIT, RAY_HIT,
	};
}


// scene 관련 //
namespace Scene
{
	enum class SceneState
	{
		NONE, START, LOADING, LOGIN, MATCH, INGAME,
	};
}


// UI 관련 //
namespace UI
{
	// UI의 ID //
	enum class ID
	{
		NONE, START_BACKGROUND, START_BUTTON, LOADING_BACKGROUND,
	};

	enum class State
	{
		NONE, ACTIVE, DEACTIVE, HOVER, ONCLICKED
	};
}


// Text 관련 //
namespace Text
{
	enum class TextFlag
	{
		DEFAULT,
	};

	// font 파일 정보 //
	const std::wstring FontFileName = _T("./resource/굴림.spritefont");
}


// sound 관련 //
namespace Sound
{
	enum class ID
	{
		NONE, BACKGROUND, EFFECT, HIT,
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
			NONE,
			DEFAULT_SPACESHIP,
			COLLISION,
			ASTEROID,
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
			NONE, START_BACKGROUND, START_BUTTON, LOADING_BACKGROUND,
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
		// shader ID //
		enum class ID
		{
			NONE, DEFAULT, ALPHA_MAP, TEXTURE, MULTI_TEXTURE, LIGHT, LIGHT_MAP, SPACESHIP, CUBE,
		};

		// File info //
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

		// shader buffer type //
		struct MatrixBufferType
		{
			DirectX::XMMATRIX World;
			DirectX::XMMATRIX View;
			DirectX::XMMATRIX Projection;
		};

		struct LightBufferType
		{
			DirectX::XMFLOAT4 AmbientColor;
			DirectX::XMFLOAT4 DiffuseColor;
			DirectX::XMFLOAT3 LightDirection;
			float padding1 = 0.f;
			DirectX::XMFLOAT4 SpecularColor;
			float SpecularPower;
			float padding2[3] = { 0.f, };
		};

		struct CameraBufferType
		{
			DirectX::XMFLOAT3 CameraPosition;
			float padding;
		};

		// shader buffer 모음 //
		struct MLC_ShaderBuffers
		{
			MatrixBufferType transform;
			LightBufferType light;
			CameraBufferType camera;
		};
	}
}