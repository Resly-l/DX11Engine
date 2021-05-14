#include "transform_component.h"
#include "entity.h"

Matrix TransformComponent::GetTransformMatrix() const
{
	Matrix mParentTransform;

	if (auto pParent = pOwner->GetParent())
	{
		if (auto pParentTransform = pParent->GetComponent<TransformComponent>())
		{
			mParentTransform =
				(pParentTransform->passdown.bScale ? Matrix::Scaling(pParentTransform->vScale) : Matrix::Identity()) *
				(pParentTransform->passdown.bRotation ? Matrix::Rotation(pParentTransform->vAngle) : Matrix::Identity()) *
				(pParentTransform->passdown.bTranslation ? Matrix::Translation(pParentTransform->vPosition) : Matrix::Identity());
		}
	}

	return Matrix::Scaling(vScale)
		* Matrix::Rotation(vAngle)
		* Matrix::Translation(vPosition)
		* mParentTransform;
}

JSON TransformComponent::ToJson() const
{
	JSON json;

	json["vPosition"] = vPosition;
	json["vAngle"] = vAngle;
	json["vScale"] = vScale;

	return json;
}

void TransformComponent::FromJson(const JSON& json)
{
	vPosition = json["vPosition"];
	vAngle = json["vAngle"];
	vScale = json["vScale"];
}

void TransformComponent::DrawWidget()
{
	ImGui::DragFloat3("position", &vPosition[0], 0.01f);
	ImGui::DragFloat3("angle", &vAngle[0], 0.01f);
	ImGui::DragFloat3("scale", &vScale[0], 0.01f);

	ImGui::NewLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("pass down");

	ImGui::Checkbox("translation", &passdown.bTranslation);
	ImGui::Checkbox("rotation", &passdown.bRotation);
	ImGui::Checkbox("scale", &passdown.bScale);
}