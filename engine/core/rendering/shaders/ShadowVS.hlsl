#define maxBones 300

cbuffer Transform : register(b1)
{
	matrix world;
};

cbuffer LightViewProjection : register(b5)
{
	matrix lightView;
	matrix lightPerspective;
};

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

float4 main(VSIn input) : SV_POSITION
{
	float3 finalPosition = input.position;

	if (useBoneTransform)
	{
		finalPosition = float3(0.0f, 0.0f, 0.0f);

		for (uint i = 0; i < 4; i++)
		{
			const matrix transform = boneTransforms[input.boneID[i]];
			const float boneWeight = input.boneWeight[i];

			finalPosition += mul(transform, float4(input.position, 1.0f)).xyz * boneWeight;
		}
	}

	return mul(mul(lightPerspective, mul(lightView, world)), float4(finalPosition, 1.0f));
}