// GLOBAL //
cbuffer MatrixBuffer : register(b0)
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};

cbuffer CameraBuffer : register(b1)
{
	float3 CameraPosition;
	float padding;
};

// TYPEDEF //
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
};

// vertex shader //
PixelInputType ColorVertexShader(VertexInputType input)
{
	PixelInputType output;

	// 위치 벡터를 4 단위로 변경
	input.position.w = 1.0f;

	// world, view, projection matrix에 대한 정점 위치 계산
	output.position = mul(input.position, WorldMatrix);
	output.position = mul(output.position, ViewMatrix);
	output.position = mul(output.position, ProjectionMatrix);

	return output;
}