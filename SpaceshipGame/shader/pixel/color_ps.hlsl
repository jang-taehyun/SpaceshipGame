// TYPEDEFS //
struct PixelInputType
{
	float4 position : SV_POSITION;

#ifdef _DEBUG
	float4 color : COLOR0;
#endif
};

// pixel shader //
float4 ColorPixelShader(PixelInputType input) : SV_TARGET
{
#ifdef _DEBUG
	return input.color;
#else
    float4 c = { 1.f, 1.f, 1.f, 0.f };
	return c;
#endif
}