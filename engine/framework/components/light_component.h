#pragma once
#include "component.h"
#include "resource.h"

class LightComponent : public Component<LightComponent>
{
public:
	struct Property
	{
		Vector position;
		Vector direction;
		float3 color = { 1.0f, 1.0f, 1.0f };
		float fIntensity = 1.0f;
		float fLinearAttenuation = 0.025f;
		float fQuadraticAttenuation = 0.003f;
		float fConstantAttenuation = 1.0f;
		float fAmbientStrength = 0.005f;
		alignas (16) BOOL bCastShadow = true;
	};

private:
	Property property;

public:
	const Property& GetProperty();

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;
};

struct LightResource : public Resource
{
	std::vector<LightComponent::Property> lightProperties;
};