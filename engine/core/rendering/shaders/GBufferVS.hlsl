#define maxBones 300

cbuffer ViewProjection : register(b0)
{
	matrix view;
	matrix perspective;
}

cbuffer Transform : register(b1)
{
	matrix world;
}

cbuffer BoneUsage : register(b6)
{
	bool useBoneTransform;
}
cbuffer BoneTransform : register(b7)
{
	matrix boneTransforms[maxBones];
}

struct VSIn
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float2 texcoord : TEXCOORD;
	uint4 boneID : BONE_ID;
	float4 boneWeight : BONE_WEIGHT;
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
	const matrix worldView = mul(view, world);
	const matrix worldViewProjection = mul(perspective, worldView);

	float3 finalPosition = input.position;
	float3 finalNormal = input.normal;
	float3 finalTangent = input.tangent;
	float3 finalBitangent = input.bitangent;

	if (useBoneTransform)
	{
		finalPosition = float3(0.0f, 0.0f, 0.0f);
		finalNormal = float3(0.0f, 0.0f, 0.0f);
		finalTangent = float3(0.0f, 0.0f, 0.0f);
		finalBitangent = float3(0.0f, 0.0f, 0.0f);

		for (uint i = 0; i < 4; i++)
		{
			const matrix transform = boneTransforms[input.boneID[i]];
			const float boneWeight = input.boneWeight[i];

			finalPosition += mul(transform, float4(input.position, 1.0f)).xyz * boneWeight;
			finalNormal += mul(transform, float4(input.normal, 0.0f)).xyz * boneWeight;
			finalTangent += mul(transform, float4(input.tangent, 0.0f)).xyz * boneWeight;
			finalBitangent += mul(transform, float4(input.bitangent, 0.0f)).xyz * boneWeight;
		}
	}

	VSOut output;

	output.position = mul(world, float4(finalPosition, 1.0f)).xyz;
	output.normal = mul(worldView, float4(finalNormal, 0.0f)).xyz;
	output.tangent = mul(worldView, float4(finalTangent, 0.0f)).xyz;
	output.bitangent = mul(worldView, float4(finalBitangent, 0.0f)).xyz;
	output.texcoord = input.texcoord;
	output.sv_position = mul(worldViewProjection, float4(finalPosition, 1.0f));

	return output;
}