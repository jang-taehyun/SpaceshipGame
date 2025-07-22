// GLOBAL //
cbuffer MatrixBuffer : register(b0)
{
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};

// TYPEDEFS //
struct VertexInputType
{
	float4 position : POSITION;
	float4 instCol0 : INSTANCE_WORLD_COLUMN0;
	float4 instCol1 : INSTANCE_WORLD_COLUMN1;
	float4 instCol2 : INSTANCE_WORLD_COLUMN2;
	float4 instCol3 : INSTANCE_WORLD_COLUMN3;

#ifdef _DEBUG
	float4 instCol4 : INSTANCE_WORLD_COLUMN4;
#endif
};

struct PixelInputType
{
	float4 position : SV_POSITION;

#ifdef _DEBUG
	float4 color : COLOR0;
#endif
};

// vertex shader //
PixelInputType ColorVertexShader(VertexInputType input)
{
	PixelInputType output;
	float4x4 WorldMatrix = 
	{
		input.instCol0,
		input.instCol1,
		input.instCol2,
		input.instCol3
	};

	// 위치 벡터를 4 단위로 변경
	input.position.w = 1.0f;

	// world, view, projection matrix에 대한 정점 위치 계산
	output.position = mul(input.position, WorldMatrix);
	output.position = mul(output.position, ViewMatrix);
	output.position = mul(output.position, ProjectionMatrix);

#ifdef _DEBUG
	// color 설정
	output.color = input.instCol4;
#endif

	return output;
}