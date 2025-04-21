// GLOBAL //
cbuffer MatrixBuffer
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};

cbuffer CameraBuffer
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
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
};

// vertex shader //
PixelInputType SpaceshipVertexShader(VertexInputType input)
{
	PixelInputType output;
	float4 WorldPosition;

	// 적절한 행렬 계산을 위해 위치 벡터를 4 단위로 변경 //
	input.position.w = 1.f;

	// world, view, projection matrix를 적용해 정점의 위치 계산 //
	output.position = mul(input.position, WorldMatrix);
	output.position = mul(output.position, ViewMatrix);
	output.position = mul(output.position, ProjectionMatrix);

	// pixel shader의 texture 좌표 저장 //
	output.tex = input.tex;

	// normal vector를 월드 공간으로 변환 //
    output.normal = mul(input.normal, (float3x3)WorldMatrix);

	// normal vector 정규화
	output.normal = normalize(output.normal);

	// world matrix 상에서 정점 위치 계산
	WorldPosition = mul(input.position, WorldMatrix);

	// 카메라의 위치와 world matrix 상에서 정점 위치를 기준으로 view direction vector를 설정
	output.viewDirection = CameraPosition.xyz - WorldPosition.xyz;

	// view direction vector 정규화
	output.viewDirection = normalize(output.viewDirection);
	// output.viewDirection = normalize(CameraPosition.xyz);

	return output;
}