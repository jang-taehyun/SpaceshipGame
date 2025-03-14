#pragma once

// -------- typedef -------- //

// shader 파일 관련
typedef struct _shader_file_info_
{
	tstring vsFileName = { 0, };
	tstring psFileName = { 0, };
	tstring vsEntryPoint = { 0, };
	tstring psEntryPoint = { 0, };
} ShaderFileInfo;

// sound 파일 관련
typedef struct _sound_info_
{
	tstring filename = { 0, };
} SoundFileInfo;

// vertex 데이터 관련
typedef struct _vertex_type_
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texture;
	DirectX::XMFLOAT3 normal;
} VertexType;

// ------------------------ //

// data //

const SoundFileInfo s_info = { _T("./data/sound01.wav") };

const tstring FontFileName = _T("./data/font01.txt");
const tstring FontTextureFileName = _T("./data/font01.tga");

const tstring CubeModelFileName = _T("./data/cat.obj");
const std::vector<tstring> CubeTextureFileNames =
{
	_T("./data/cat.png")
};

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

// ------------------------ //

// 해상도 //
const int WIDTH = 1200;
const int HEIGHT = 800;