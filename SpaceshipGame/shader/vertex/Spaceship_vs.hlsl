// GLOBAL //
cbuffer MatrixBuffer : register(b0)
{
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};

cbuffer CameraBuffer : register(b1)
{
	float3 CameraPosition;
	float padding;
};

// TYPEDEFS //
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 instCol0 : INSTANCE_WORLD_COLUMN0;
	float4 instCol1 : INSTANCE_WORLD_COLUMN1;
	float4 instCol2 : INSTANCE_WORLD_COLUMN2;
	float4 instCol3 : INSTANCE_WORLD_COLUMN3;
	float4 instCol4 : INSTANCE_WORLD_COLUMN4;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
	float4 color : COLOR0;
};

// vertex shader //
PixelInputType SpaceshipVertexShader(VertexInputType input)
{
	PixelInputType output;
	float4 WorldPosition;
	float4x4 WorldMatrix = 
	{
		input.instCol0,
		input.instCol1,
		input.instCol2,
		input.instCol3
	};
	float3 normalW;

	// 적절한 행렬 계산을 위해 위치 벡터를 4 단위로 변경 //
	input.position.w = 1.f;

	// world, view, projection matrix를 적용해 정점의 위치 계산 //
	output.position = mul(input.position, WorldMatrix);
	output.position = mul(output.position, ViewMatrix);
	output.position = mul(output.position, ProjectionMatrix);

	// pixel shader의 texture 좌표 저장 //
	output.tex = input.tex;

	// normal 변환 //
	normalW = mul((float3x3)WorldMatrix, input.normal);
	output.normal = normalize(normalW);

	// view direction 연산 //
	WorldPosition = mul(input.position, WorldMatrix);
	output.viewDirection = normalize(CameraPosition.xyz - WorldPosition.xyz);

	// color 설정
	output.color = input.instCol4;

	return output;
}