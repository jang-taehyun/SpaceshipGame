// GLOBAL //
cbuffer PerFrameBuffer
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};

// TYPEDEFS //
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

// vertex shader //
PixelInputType FontVertexShader(VertexInputType input)
{
	PixelInputType output;

	// position vector를 4단위로 지정 //
	input.position.w = 1.f;

	// vertex의 위치 계산 //
	output.position = mul(input.position, WorldMatrix);
	output.position = mul(output.position, ViewMatrix);
	output.position = mul(output.position, ProjectionMatrix);

	// texture 좌표 지정 //
	output.tex = input.tex;

	return output;
}