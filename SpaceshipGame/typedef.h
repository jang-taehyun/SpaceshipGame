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
	std::wstring vsFileName;
	std::wstring psFileName;
	std::string vsEntryPoint;
	std::string psEntryPoint;
} ShaderFileInfo;

// sound 파일 관련
typedef struct _sound_info_
{
	std::wstring filename;
} SoundFileInfo;

// vertex 데이터 관련
// typedef struct _vertex_type_
// {
// 	DirectX::XMFLOAT3 position;
// 	DirectX::XMFLOAT2 texture;
// 	DirectX::XMFLOAT3 normal;
// } VertexType;

// transform matrix(world matrix, view matrix, projection) 관련
typedef struct _transform_matrix_data_
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
} TransformMatrixData;

// affine 데이터(이동, 회전, 스케일) 관련
typedef struct _affine_info_
{
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 rotation;
	DirectX::XMFLOAT4 scale;
} AffineInfo;

// 3D Model 데이터 관련
typedef struct _model_info_
{
	// 3D Model 파일 이름, 경로
	std::wstring ModelFileName;

	// texture 파일 이름, 경로
	std::vector<std::wstring> TextureFileNames;

	// shader 파일 이름, 경로 및 entry point
	ShaderFileInfo ShaderInfo;

} ModelInfo;