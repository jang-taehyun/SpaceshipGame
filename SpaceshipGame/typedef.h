#pragma once

// Error 관련
typedef struct _error_content_
{
	tstring title;
	tstring contents;
	HRESULT errorCode = S_OK;
} ErrorContent;

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