#pragma once

// system 관련 //
namespace System
{
	// 해상도 관련
	struct RESOLUTION
	{
		UINT WIDTH;
		UINT HEIGHT;
	};

	extern UINT RESOLUTION_COUNT;
	extern UINT CURRENT_RESOLUTION_INDEX;
	extern RESOLUTION RESOLUTIONS[2];

	// Graphics 설정
	extern bool FULL_SCREEN;
	extern bool VSYNC_ENABLED;
	extern float SCREEN_DEPTH;
	extern float SCREEN_NEAR;

	// MOUSE 감도 설정
	extern float MOUSE_SENSITIVITY;
	
	// 프로그램 동작 설정
	extern bool RUNNING;
	extern HWND hWnd;
	extern HINSTANCE hInst;

	// input 관련
	enum class KEYSTATE
	{
		NONE,
		TAP,
		HOLD,
		AWAY,
	};

	enum class KEY
	{
		W,
		A,
		S,
		D,

		UP_ARROW,
		LEFT_ARROW,
		DOWN_ARROW,
		RIGHT_ARROW,

		ESC,
		SPACEBAR,

		MOUSE_LEFT,
		MOUSE_RIGHT,
		MOUSE_CENTER,

		LAST
	};
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
		CAMERA,
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

	// affine 데이터(이동, 회전, 스케일) 관련
	struct AffineInfo
	{
		DirectX::XMFLOAT4 position = {};
		DirectX::XMFLOAT4 rotation = {};
		DirectX::XMFLOAT4 scale = {};
	};
}


// scene 관련 //
namespace Scene
{
	enum class ID
	{
		NONE, START, LOADING, MODE_CHOOSE, SINGLE_MODE, MULTI_MODE, 
	};
}


// UI 관련 //
namespace UI
{
	// UI의 종류 ID
	enum class ID
	{
		NONE, STATIC, BACKGROUND, BUTTON,
	};

	// UI 상태
	enum class UIState
	{
		NONE, ACTIVE, APPEAR, 
	};

	// button 상태
	enum class ButtonState
	{
		NONE, HOVER, ONPRESSED, ONCLICKED, 
	};

	extern const float ExtendSize;
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
	enum class ID
	{
		NONE, BACKGROUND, EFFECT, HIT,
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
			COLLISION,
			DEFAULT_SPACESHIP,
			ASTEROID,
		};

		// instance buffer type
		struct InstanceBufferType
		{
			DirectX::XMFLOAT4X4 world = {};
			DirectX::XMFLOAT4 color = {};
		};

		// PTN vertex type
		struct PTN_VertexType
		{
			DirectX::XMFLOAT4 position = {};
			DirectX::XMFLOAT2 texture = {};
			DirectX::XMFLOAT3 normal = {};
		};

		// P vertex type
		struct P_VertexType
		{
			DirectX::XMFLOAT4 position = {};
		};
	}

	// Terrain 관련 //
	namespace Terrain
	{
		enum class TerrainID
		{
			NONE, DEFAULT, 
		};

		enum class SkyDomeID
		{
			NONE, DEFAULT,
		};
	}

	// UI texture 관련 //
	namespace Texture
	{
		enum class UITextureID
		{
			NONE, LOGO, START_BACKGROUND, START_BUTTON, ECS_POPUP_BACKGROUND, ECS_POPUP,
		};
	}

	// shader 관련 //
	namespace Shader
	{
		enum class ID
		{
			NONE, DEFAULT_SPACESHIP, CUBE, TERRAIN, 
		};

		// shader buffer type //
		struct MatrixBufferType
		{
			DirectX::XMMATRIX View = {};
			DirectX::XMMATRIX Projection = {};
		};

		struct LightBufferType
		{
			DirectX::XMFLOAT4 AmbientColor = {};
			DirectX::XMFLOAT4 DiffuseColor = {};
			DirectX::XMFLOAT3 LightDirection = {};
			float padding1 = 0.f;
			DirectX::XMFLOAT4 SpecularColor = {};
			float SpecularPower = 0.f;
			float padding2[3] = { 0.f, };
		};

		struct CameraBufferType
		{
			DirectX::XMFLOAT3 CameraPosition = {};
			float padding = 0.f;
		};

		// shader buffer 모음 //

		// 모든 buffer 모음
		struct BuffersData
		{
			MatrixBufferType transform = {};
			LightBufferType light = {};
			CameraBufferType camera = {};
		};

		// cube 전용(matrix만 존재)
		struct M_ShaderBuffer
		{
			MatrixBufferType transform = {};
		};

		// matrix, light, camera
		struct MLC_ShaderBuffers
		{
			MatrixBufferType transform = {};
			LightBufferType light = {};
			CameraBufferType camera = {};
		};

		// matrix, light
		struct ML_ShaderBuffers
		{
			MatrixBufferType transform = {};
			LightBufferType light = {};
		};
	}

	// font 관련 //
	namespace Font
	{
		enum class ID
		{
			NONE, DEFAULT, 
		};
	}

	// loader 관련 //
	namespace Loader
	{
		// shader 파일 정보
		struct ShaderFileInfo
		{
			std::wstring vsFileName;
			std::wstring psFileName;
			std::string vsEntryPoint;
			std::string psEntryPoint;
		};
	}
}