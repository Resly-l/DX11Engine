cbuffer ViewProjection : register(b0)
{
	matrix view;
	matrix projection;
};

cbuffer Transform : register(b1)
{
	matrix mWorld;
}

float4 main(float3 position : POSITION) : SV_POSITION
{
	return mul(mul(projection, mul(view, mWorld)), float4(position, 1.0f));
}