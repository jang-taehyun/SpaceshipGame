#pragma once

// Error 관련
typedef struct _error_content_
{
	std::wstring title;
	std::wstring contents;
	HRESULT errorCode = S_OK;
} ErrorContent;

// shader 파일 관련
typedef struct _shader_file_info_
{
	std::wstring vsFileName = { 0, };
	std::wstring psFileName = { 0, };
	std::string vsEntryPoint = { 0, };
	std::string psEntryPoint = { 0, };
} ShaderFileInfo;

// sound 파일 관련
typedef struct _sound_info_
{
	std::wstring filename = { 0, };
} SoundFileInfo;

// vertex 데이터 관련
typedef struct _vertex_type_
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texture;
	DirectX::XMFLOAT3 normal;
} VertexType;