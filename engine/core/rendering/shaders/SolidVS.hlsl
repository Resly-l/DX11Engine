cbuffer ViewProjection : register(b0)
{
	matrix view;
	matrix perspective;
};

cbuffer Transform : register(b1)
{
	matrix world;
}

float4 main(float3 position : POSITION) : SV_POSITION
{
	return mul(mul(perspective, mul(view, world)), float4(position, 1.0f));
}