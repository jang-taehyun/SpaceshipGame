// GLOBAL //
cbuffer MatrixBuffer
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
PixelInputType TextureVertexShader(VertexInputType input)
{
	PixelInputType output;

	// ������ ��� ����� ���� ��ġ ���͸� 4 ������ ���� //
	input.position.w = 1.f;

	// world, view, projection matrix�� ������ ������ ��ġ ��� //
	output.position = mul(input.position, WorldMatrix);
	output.position = mul(output.position, ViewMatrix);
	output.position = mul(output.position, ProjectionMatrix);

	// pixel shader�� texture ��ǥ ���� //
	output.tex = input.tex;

	return output;
}