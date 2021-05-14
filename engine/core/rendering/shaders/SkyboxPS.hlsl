TextureCube skyTexture : register(t4);
SamplerState textureSampler : register(s0);

float4 main(float3 pos : POSITION) : SV_TARGET
{
	return skyTexture.Sample(textureSampler, pos);
}