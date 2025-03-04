// GLOBAL //
cbuffer MatrixBuffer
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};

// TYPEDEF //
struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

// vertex shader //
PixelInputType ColorVertexShader(VertexInputType input)
{
	PixelInputType output;

	// ��ġ ���͸� 4 ������ ����
	input.position.w = 1.0f;

	// world, view, projection matrix�� ���� ���� ��ġ ���
	output.position = mul(input.position, WorldMatrix);
	output.position = mul(output.position, ViewMatrix);
	output.position = mul(output.position, ProjectionMatrix);

	// pixel shader�� ����� �Է� ���� ����
	output.color = input.color;

	return output;
}