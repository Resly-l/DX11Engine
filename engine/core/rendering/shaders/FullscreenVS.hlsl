struct VSOut
{
	float2 texcoord : TEXCOORD;
	float4 sv_position : SV_POSITION;
};

VSOut main(float3 position : POSITION)
{
	VSOut output;

	output.texcoord = float2((position.x + 1.0f) / 2.0f, -(position.y - 1.0f) / 2.0f);
	output.sv_position = float4(position, 1.0f);

	return output;
}