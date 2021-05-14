cbuffer Material : register(b2)
{
	float4 albedo;
	float4 specular;
	bool bUseAlbedoMap;
	bool bUseNormalMap;
	bool bUseSpecularMap;
	float fShininess;
};

Texture2D normalMap : register(t1);
Texture2D albedoMap : register(t2);
Texture2D specularMap : register(t3);
SamplerState textureSampler : register(s0);

struct PSIn
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float2 texcoord : TEXCOORD;
};

struct PSOut
{
	float4 position : SV_TARGET0;
	float4 normal : SV_TARGET1;
	float4 albedo : SV_TARGET2;
	float4 specular : SV_TARGET3;
};

PSOut main(PSIn input)
{
	PSOut output;

	output.position = float4(input.position, 0.0f);

	output.normal = float4(normalize(input.normal), 1.0f);
	if (bUseNormalMap)
	{
		float3x3 mWorldToTangent = float3x3(normalize(input.tangent), normalize(input.bitangent), normalize(input.normal));
		float3 sampledNormal = normalMap.Sample(textureSampler, input.texcoord).rgb * 2.0f - 1.0f;
		output.normal = float4(normalize(mul(sampledNormal, mWorldToTangent)), 0.0f);
	}

	output.albedo = bUseAlbedoMap ? albedoMap.Sample(textureSampler, input.texcoord) : float4(0.0f, 0.0f, 0.0f, 1.0f);
	if (output.albedo.a <= 0.1f) // alpha testing
	{
		discard;
	}

	output.specular = bUseSpecularMap ? specularMap.Sample(textureSampler, input.texcoord) : float4(specular.rgb, 1.0f);
	output.specular.a = fShininess;

	return output;
}