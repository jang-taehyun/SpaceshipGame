#pragma once

// system 관련 //
namespace System
{
	// 해상도
	extern UINT WIDTH;
	extern UINT HEIGHT;

	// Graphics 설정
	extern bool FULL_SCREEN;
	extern bool VSYNC_ENABLED;
	extern float SCREEN_DEPTH;
	extern float SCREEN_NEAR;

	// MOUSE 감도 설정
	extern float MOUSE_SENSITIVITY;
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

	// affine 데이터(이동, 회전, 스케일) 관련
	struct AffineInfo
	{
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT4 rotation;
		DirectX::XMFLOAT4 scale;
	};
}


// scene 관련 //
namespace Scene
{
	enum class SceneID
	{
		NONE, START, LOADING, LOGIN, MATCH, INGAME, MODE_CHOOSE,
	};
}


// UI 관련 //
namespace UI
{
	// UI의 종류 ID
	enum class ID
	{
		NONE, DEFAULT, BACKGROUND, BUTTON,
	};

	// UI 상태
	enum class UIState
	{
		NONE, ACTIVE, DEACTIVE, DISAPPEAR, 
	};

	// button 상태
	enum class ButtonState
	{
		NONE, HOVER, ONPRESSED, ONCLICKED, 
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
	extern UINT SoundIDCount;
	enum class ID
	{
		NONE, BACKGROUND, EFFECT, HIT,
	};

	extern std::map<ID, const std::wstring> SoundFileList;
}

// Graphic 관련 //
namespace Graphic
{
	// 3D model 관련 //
	namespace Model
	{
		extern UINT ModelIDCount;
		enum class ID
		{
			NONE,
			DEFAULT_SPACESHIP,
			COLLISION,
			ASTEROID,
		};

		extern std::map<ID, const std::string> ModelFileList;
		extern std::map<ID, const std::wstring> ModelTexturePathList;

		// instance buffer type
		struct InstanceBufferType
		{
			DirectX::XMFLOAT4X4 world = {};

#ifdef _DEBUG
			DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
#endif // DEBUG
		};

		// PTN vertex type
		struct PTN_VertexType
		{
			DirectX::XMFLOAT4 position = {};
			DirectX::XMFLOAT2 texture = {};
			DirectX::XMFLOAT3 normal = {};
		};
	}

	// UI texture 관련 //
	namespace Texture
	{
		extern UINT UITextureIDCount;
		enum class UITextureID
		{
			NONE, START_BACKGROUND, START_BUTTON,
		};

		// UI texture의 파일 정보 //
		extern std::map<UITextureID, const std::wstring> UITextureFileList;
	}

	// shader 관련 //
	namespace Shader
	{
		extern UINT ShaderIDCount;
		enum class ID
		{
			NONE, DEFAULT_SPACESHIP, CUBE, ALPHA_MAP, TEXTURE, MULTI_TEXTURE, LIGHT, LIGHT_MAP,
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
	}

	// font 관련 //
	namespace Font
	{
		extern UINT FontIDCount;
		enum class ID
		{
			NONE, DEFAULT, 
		};

		extern std::map<ID, const std::wstring> FontFileList;
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