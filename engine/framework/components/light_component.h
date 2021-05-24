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
		float intensity = 1.0f;
		float linearAttenuation = 0.025f;
		float quadraticAttenuation = 0.003f;
		float constantAttenuation = 1.0f;
		float ambientStrength = 0.005f;
		alignas (16) BOOL castShadow = true;
	};

public:
	const Property& GetProperty();

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;

private:
	Property property;
};

struct LightResource : public Resource
{
	std::vector<LightComponent::Property> lightProperties;
};