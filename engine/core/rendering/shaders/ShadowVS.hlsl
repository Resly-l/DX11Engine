cbuffer Transform : register(b1)
{
	matrix mWorld;
};

cbuffer LightViewProjection : register(b5)
{
	matrix mLightView;
	matrix mLightProjection;
};

struct VSIn
{
	float3 position : POSITION;
};

float4 main(VSIn input) : SV_POSITION
{
	return mul(mul(mLightProjection, mul(mLightView, mWorld)), float4(input.position, 1.0f));
}