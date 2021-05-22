#define uMaxLights 50

cbuffer ViewProjection : register(b0)
{
	matrix view;
	matrix projection;
};

struct LightProperty
{
	float3 position;
	float3 direction;
	float3 color;
	float intensity;
	float linearAttenuation;
	float quadraticAttenuation;
	float constantAttenuation;
	float ambientStrength;
	bool castShadow;
};

cbuffer Light : register(b3)
{
	LightProperty lights[uMaxLights];
};
cbuffer LightCount : register(b4)
{
	uint uNumLights;
}

Texture2D positionMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D albedoMap : register(t2);
Texture2D specularMap : register(t3);
SamplerState textureSampler : register(s0);

TextureCube shadowMap[uMaxLights]: register(t5);
SamplerComparisonState shadowSampler : register(s1);

float4 main(float2 texcoord : TEXCOORD) : SV_TARGET
{
	const float3 worldPosition = positionMap.Sample(textureSampler, texcoord).rgb;
	const float3 viewPosition = mul(view, float4(worldPosition, 1.0f)).rgb;
	const float3 normal = normalMap.Sample(textureSampler, texcoord).rgb;
	const float3 albedo = albedoMap.Sample(textureSampler, texcoord).rgb;
	// rgb = specular color, a = specular shininess
	const float4 specular = specularMap.Sample(textureSampler, texcoord);

	float3 finalColor = float3(0.0f, 0.0f, 0.0f);

	[unroll]
	for (uint uLight = 0u; uLight < uNumLights; uLight++)
	{
		const float3 lightViewPosition = mul(view, float4(lights[uLight].position, 1.0f)).xyz;
		const float3 vToLight = lightViewPosition - viewPosition;
		const float fLenToLight = length(vToLight);

		// attenuation = 1.0f / a + b * d + c * sq(d)
		// a : constant attenuation
		// b : linear attenuation
		// c : quadratic attenuation
		// d : distance
		const float fAttenuation = 1.0f / (lights[uLight].constantAttenuation + lights[uLight].linearAttenuation * fLenToLight + lights[uLight].quadraticAttenuation * (fLenToLight * fLenToLight));

		const float3 diffuseReflection = albedo * lights[uLight].color * lights[uLight].intensity * fAttenuation * max(0.0f, dot(normal, vToLight / fLenToLight));
		const float3 specularReflection = specular.rgb * lights[uLight].intensity * fAttenuation * pow(max(0.0f, dot(normalize(-viewPosition), normalize(reflect(-vToLight, normal)))), specular.a);
		const float3 ambient = lights[uLight].color * lights[uLight].ambientStrength;

		float fShadowLevel = 1.0f;
		if (lights[uLight].castShadow)
		{
			static const float zn = 0.5f;
			static const float zf = 100.0f;
			static const float r23 = zf / (zf - zn);
			static const float r33 = -zn * zf / (zf - zn);

			const float3 shadowPos = worldPosition - lights[uLight].position;
			const float3 m = abs(shadowPos);
			const float major = max(m.x, max(m.y, m.z));

			const float fShadowDepth = (r23 * major + r33) / major;
			fShadowLevel = shadowMap[uLight].SampleCmpLevelZero(shadowSampler, shadowPos, fShadowDepth);
		}

		finalColor += saturate((diffuseReflection + specularReflection + ambient) * fShadowLevel);
	}

	return float4(finalColor, 1.0f);
}