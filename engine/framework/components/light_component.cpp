#include "light_component.h"
#include "transform_component.h"
#include "entity.h"

const LightComponent::Property& LightComponent::GetProperty()
{
	if (auto pTransformComponent = GetOwner()->GetComponent<TransformComponent>())
	{
		Matrix transform = pTransformComponent->GetAbsoluteTransform();

		property.position = transform.m.r[3];
		property.direction = (Vector(0.0f, 0.0f, 1.0f, 0.0f) * transform).GetNormalized3();
	}
	else
	{
		property.position = {};
		property.direction = { 0.0f, 0.0f, 1.0f, 0.0f };
	}

	return property;
}

JSON LightComponent::ToJson() const
{
	JSON json;

	json["color_x"] = property.color.x;
	json["color_y"] = property.color.y;
	json["color_z"] = property.color.z;

	json["intensity"] = property.intensity;
	json["linearAttenuation"] = property.linearAttenuation;
	json["quadraticAttenuation"] = property.quadraticAttenuation;
	json["constantAttenuation"] = property.constantAttenuation;
	json["castShadow"] = property.castShadow;

	return json;
}

void LightComponent::FromJson(const JSON& json)
{
	property.color.x = json["color_x"];
	property.color.y = json["color_y"];
	property.color.z = json["color_z"];

	property.intensity = json["intensity"];
	property.linearAttenuation = json["linearAttenuation"];
	property.quadraticAttenuation = json["quadraticAttenuation"];
	property.constantAttenuation = json["constantAttenuation"];
	property.castShadow = json["castShadow"];
}

void LightComponent::DrawWidget()
{
	ImGui::ColorEdit3("color", &property.color.x);
	ImGui::DragFloat("intensity", &property.intensity, 0.01f);

	ImGui::NewLine();
	ImGui::DragFloat("linear attenuation", &property.linearAttenuation, 0.01f);
	ImGui::DragFloat("quadratic attenuation", &property.quadraticAttenuation, 0.01f);
	ImGui::DragFloat("constant attenuation", &property.constantAttenuation, 0.01f);

	ImGui::NewLine();
	ImGui::DragFloat("ambient strength", &property.ambientStrength, 0.01f);

	ImGui::NewLine();
	ImGui::Checkbox("cast shadow", reinterpret_cast<bool*>(&property.castShadow));
}