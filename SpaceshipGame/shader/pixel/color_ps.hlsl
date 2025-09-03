// TYPEDEFS //
struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
};

// pixel shader //
float4 ColorPixelShader(PixelInputType input) : SV_TARGET
{
	return input.color;
}