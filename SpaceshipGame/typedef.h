#pragma once

namespace Graphic
{
	namespace Loader
	{
		// shader 정보 //
		typedef struct _shader_file_info_
		{
			std::wstring vsFileName;
			std::wstring psFileName;
			std::string vsEntryPoint;
			std::string psEntryPoint;
		} ShaderFileInfo;
	}
}

// transform matrix(world matrix, view matrix, projection) 관련
typedef struct _transform_matrix_data_
{
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