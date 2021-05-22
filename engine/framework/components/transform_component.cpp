#include "transform_component.h"
#include "entity.h"

Matrix TransformComponent::GetTransformMatrix() const
{
	Matrix mParentTransform;

	if (auto pParent = GetOwner()->GetParent())
	{
		if (auto pParentTransform = pParent->GetComponent<TransformComponent>())
		{
			mParentTransform =
				(pParentTransform->passScale ? Matrix::Scaling(pParentTransform->scale) : Matrix::Identity()) *
				(pParentTransform->passRotation ? Matrix::Rotation(pParentTransform->angle) : Matrix::Identity()) *
				(pParentTransform->passTranslation ? Matrix::Translation(pParentTransform->position) : Matrix::Identity());
		}
	}

	return Matrix::Scaling(scale)
		* Matrix::Rotation(angle)
		* Matrix::Translation(position)
		* mParentTransform;
}

JSON TransformComponent::ToJson() const
{
	JSON json;

	json["position"] = position;
	json["angle"] = angle;
	json["scale"] = scale;

	return json;
}

void TransformComponent::FromJson(const JSON& json)
{
	position = json["position"];
	angle = json["angle"];
	scale = json["scale"];
}

void TransformComponent::DrawWidget()
{
	ImGui::DragFloat3("position", &position[0], 0.01f);
	ImGui::DragFloat3("angle", &angle[0], 0.01f);
	ImGui::DragFloat3("scale", &scale[0], 0.01f);

	ImGui::NewLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("pass down");
	ImGui::Checkbox("translation", &passTranslation);
	ImGui::Checkbox("rotation", &passRotation);
	ImGui::Checkbox("scale", &passScale);
}