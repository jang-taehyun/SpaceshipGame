// GLOBAL //
cbuffer MatrixBuffer : register( b0 )
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
PixelInputType LightVertexShader(VertexInputType input)
{
	PixelInputType output = (PixelInputType)0;

	// ������ ��� ����� ���� position vector�� 4 ������ ����
	input.position.w = 1.f;

	// matrix ScaleMatrix = float4x4(
    // 0.5f, 0.0f, 0.0f, 0.0f,
    // 0.0f, 0.5f, 0.0f, 0.0f,
    // 0.0f, 0.0f, 0.5f, 0.0f,
    // 0.0f, 0.0f, 0.0f, 1.0f);

	// WorldMatrix�� ScaleMatrix ���ϱ�
    // matrix ScaledWorldMatrix = mul(WorldMatrix, ScaleMatrix);

	// world, view, projection matrix�� ������ vertex�� ��ġ ����
	// output.position = mul(input.position, ScaledWorldMatrix);
	output.position = mul(input.position, WorldMatrix);
	output.position = mul(output.position, ViewMatrix);
	output.position = mul(output.position, ProjectionMatrix);

	// pixel shader�� texture ��ǥ ����
	output.tex = input.tex;

	// world matrix �������� normal vector ��ȯ
	output.normal = mul(input.normal, (float3x3)WorldMatrix);
	// output.normal = mul(input.normal, (float3x3)ScaledWorldMatrix);

	// normal vector ����ȭ
	output.normal = normalize(output.normal);

	// world matrix �󿡼� ���� ��ġ ���
	float4 WorldPosition = mul(input.position, WorldMatrix);

	// ī�޶��� ��ġ�� world matrix �󿡼� ���� ��ġ�� �������� view direction vector�� ����
	output.viewDirection = CameraPosition.xyz - WorldPosition.xyz;

	// view direction vector ����ȭ
	output.viewDirection = normalize(output.viewDirection);

	return output;
}