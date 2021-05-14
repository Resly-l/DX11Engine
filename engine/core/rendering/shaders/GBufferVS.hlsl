cbuffer ViewProjection : register(b0)
{
	matrix mView;
	matrix mProjection;
};

cbuffer Transform : register(b1)
{
	matrix mWorld;
}

struct VSIn
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float2 texcoord : TEXCOORD;
};

struct VSOut
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float2 texcoord : TEXCOORD;
	float4 sv_position : SV_POSITION;
};

VSOut main(VSIn input)
{
	const matrix mWorldView = mul(mView, mWorld);
	const matrix mWorldViewProjection = mul(mProjection, mWorldView);

	VSOut output;

	output.position = mul(mWorld, float4(input.position, 1.0f)).xyz;
	output.normal = mul(mWorldView, float4(input.normal, 0.0f)).xyz;
	output.tangent = mul(mWorldView, float4(input.tangent, 0.0f)).xyz;
	output.bitangent = mul(mWorldView, float4(input.bitangent, 0.0f)).xyz;
	output.texcoord = input.texcoord;
	output.sv_position = mul(mWorldViewProjection, float4(input.position, 1.0f));

	return output;
}