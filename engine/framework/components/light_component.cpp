#include "light_component.h"
#include "transform_component.h"
#include "entity.h"

const LightComponent::Property& LightComponent::GetProperty()
{
	if (auto pTransformComponent = pOwner->GetComponent<TransformComponent>())
	{
		Matrix mTransform = pTransformComponent->GetTransformMatrix();

		property.position = mTransform.m.r[3];
		property.direction = (Vector(0.0f, 0.0f, 1.0f, 0.0f) * mTransform).GetNormalized3();
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

	json["fIntensity"] = property.fIntensity;
	json["fLinearAttenuation"] = property.fLinearAttenuation;
	json["fQuadraticAttenuation"] = property.fQuadraticAttenuation;
	json["fConstantAttenuation"] = property.fConstantAttenuation;
	json["bCastShadow"] = property.bCastShadow;

	return json;
}

void LightComponent::FromJson(const JSON& json)
{
	property.color.x = json["color_x"];
	property.color.y = json["color_y"];
	property.color.z = json["color_z"];

	property.fIntensity = json["fIntensity"];
	property.fLinearAttenuation = json["fLinearAttenuation"];
	property.fQuadraticAttenuation = json["fQuadraticAttenuation"];
	property.fConstantAttenuation = json["fConstantAttenuation"];
	property.bCastShadow = json["bCastShadow"];
}

void LightComponent::DrawWidget()
{
	ImGui::ColorEdit3("color", &property.color.x);
	ImGui::DragFloat("intensity", &property.fIntensity, 0.01f);

	ImGui::NewLine();
	ImGui::DragFloat("linear attenuation", &property.fLinearAttenuation, 0.01f);
	ImGui::DragFloat("quadratic attenuation", &property.fQuadraticAttenuation, 0.01f);
	ImGui::DragFloat("constant attenuation", &property.fConstantAttenuation, 0.01f);

	ImGui::NewLine();
	ImGui::DragFloat("ambient strength", &property.fAmbientStrength, 0.01f);

	ImGui::NewLine();
	ImGui::Checkbox("cast shadow", reinterpret_cast<bool*>(&property.bCastShadow));
}