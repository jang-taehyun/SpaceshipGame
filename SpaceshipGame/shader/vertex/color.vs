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

	// 위치 벡터를 4 단위로 변경
	input.position.w = 1.0f;

	// world, view, projection matrix에 대한 정점 위치 계산
	output.position = mul(input.position, WorldMatrix);
	output.position = mul(output.position, ViewMatrix);
	output.position = mul(output.position, ProjectionMatrix);

	// pixel shader가 사용할 입력 색상 저장
	output.color = input.color;

	return output;
}