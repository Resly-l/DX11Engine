cbuffer ViewProjection : register(b0)
{
	matrix view;
	matrix perspective;
};

struct VSOut
{
	float3 pos : POSITION;
	float4 sv_pos : SV_POSITION;
};

VSOut main(float3 pos : POSITION)
{
	VSOut output;

	output.pos = pos;
	output.sv_pos = mul(mul(perspective, view), float4(pos, 0.0f));
	output.sv_pos.z = output.sv_pos.w;

	return output;
}