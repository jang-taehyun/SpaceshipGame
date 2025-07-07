#pragma once

#include <map>
#include "typedef.h"

// system 관련 //
namespace System
{
	// 해상도
	const int WIDTH = 1200;
	const int HEIGHT = 800;

	// Graphics 설정
	const bool FULL_SCREEN = false;
	const bool VSYNC_ENABLED = true;
	const float SCREEN_DEPTH = 1000.0f;
	const float SCREEN_NEAR = 0.1f;

	// MOUSE 감도 설정
	float MOUSE_SENSITIVITY = 0.0015f;
}

// object 관련 //
namespace Object
{
	// object의 종류 ID
	enum class ID
	{
		NONE,
		ACTOR,
		COLLISION,
	};

	// 이동 상태 정보
	enum class MoveState
	{
		MOVE_FORWARD, MOVE_BACKWARD, MOVE_RIGHT, MOVE_LEFT,
	};

	// collision 상태 정보
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
	// UI의 종류 ID
	enum class ID
	{
		NONE, DEFAULT, BUTTON, DYNAMIC, 
	};

	enum class State
	{
		NONE, ACTIVE, DEACTIVE, HOVER, ONCLICKED, DISAPPEAR, 
	};
}


// Text 관련 //
namespace Text
{
	enum class ID
	{
		NONE, DEFAULT,
	};
}


// sound 관련 //
namespace Sound
{
	const int SoundIDCount = 3;
	enum class ID
	{
		NONE, BACKGROUND, EFFECT, HIT,
	};

	std::map<ID, const std::wstring> SoundFileList =
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
		const int ModelIDCount = 3;
		enum class ID
		{
			NONE,
			DEFAULT_SPACESHIP,
			COLLISION,
			ASTEROID,
		};

		std::map<ID, const std::wstring> ModelFileList = {
			{ ID::COLLISION, _T("./data/cube.txt") },
			{ ID::DEFAULT_SPACESHIP, _T("./data/aircraft.txt") },
		};

		// PTN vertex type
		struct PTN_VertexType
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT2 texture;
			DirectX::XMFLOAT3 normal;
		};
	}

	// UI texture 관련 //
	namespace Texture
	{
		const int UITextureIDCount = 3;
		enum class UITextureID
		{
			NONE, START_BACKGROUND, START_BUTTON,
		};

		// UI texture의 파일 정보 //
		std::map<UITextureID, const std::wstring> UITextureFileList =
		{
			{ UITextureID::START_BACKGROUND, _T("./resource/StartBackground.jpg") },
			{ UITextureID::START_BUTTON, _T("./resource/button.png") }
		};
	}

	// shader 관련 //
	namespace Shader
	{
		const int ShaderIDCount = 3;
		enum class ID
		{
			NONE, DEFAULT_SPACESHIP, CUBE, ALPHA_MAP, TEXTURE, MULTI_TEXTURE, LIGHT, LIGHT_MAP,
		};

		// shader buffer type //
		struct MatrixBufferType
		{
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

		// 모든 buffer 모음
		struct BuffersData
		{
			MatrixBufferType transform;
			LightBufferType light;
			CameraBufferType camera;
		};

		// matrix, light, camera
		struct MLC_ShaderBuffers
		{
			MatrixBufferType transform;
			LightBufferType light;
			CameraBufferType camera;
		};
	}

	// font 관련 //
	namespace Font
	{
		const int FontIDCount = 2;
		enum class ID
		{
			NONE, DEFAULT, 
		};

		std::map<ID, const std::wstring> FontList =
		{
			{ ID::DEFAULT, _T("./resource/굴림.spritefont") },
		};
	}
}