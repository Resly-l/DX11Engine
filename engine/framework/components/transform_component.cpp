#include "transform_component.h"
#include "entity.h"
#include "model_component.h"
#include "rendering/constant_buffer.h"

std::unique_ptr<ConstantBuffer> TransformComponent::pConstantBuffer;

TransformComponent::TransformComponent()
{
	// delaying initialization to be initialized after renderer
	if (pConstantBuffer == nullptr)
	{
		pConstantBuffer = std::make_unique<ConstantBuffer>(ConstantType::ctINSTANCE_TRANSFORM, ShaderBindFlag::bfBIND_VS, Matrix::Identity());
	}
}

void TransformComponent::SetAbsolutePosition(const Vector& position)
{
	SetRelativePosition(position - GetParentAbsoluteTransform()[3]);
}

void TransformComponent::SetAbsoluteAngle(const Vector& angle)
{
	SetRelativeAngle(angle - GetParentAbsoluteTransform().Decompose().angle);
}

void TransformComponent::SetAbsoluteScale(const Vector& scale)
{
	const auto parentTransform = GetParentAbsoluteTransform();

	const float scaleX = Vector(parentTransform[0][0], parentTransform[0][1], parentTransform[0][2], 0.0f).Length3();
	const float scaleY = Vector(parentTransform[1][0], parentTransform[1][1], parentTransform[1][2], 0.0f).Length3();
	const float scaleZ = Vector(parentTransform[2][0], parentTransform[2][1], parentTransform[2][2], 0.0f).Length3();

	SetRelativeScale(scale - Vector(scaleX, scaleY, scaleX, 0.0f));
}

void TransformComponent::LookAt(const Vector& position)
{
	const auto transform = GetAbsoluteTransform();

	Vector direction = position - transform[3];
	Vector rightSide = Vector(direction.x, 0.0f, direction.z, 0.0f) * Matrix::Rotation({ 0.0f, (float)pi * 0.5f, 0.0f, 0.0f });
	if (abs(abs(direction.y) - 1.0f) < std::numeric_limits<float>::epsilon())
	{
		rightSide = Vector(1.0f, 0.0f, 0.0f, 0.0f) * transform;
	}
	Vector upSide = direction.Cross3(rightSide);

	Matrix rotation;
	rotation[0] = rightSide.GetNormalized3();
	rotation[1] = upSide.GetNormalized3();
	rotation[2] = direction.GetNormalized3();

	SetAbsoluteAngle(rotation.Decompose().angle);
}

Matrix TransformComponent::GetParentAbsoluteTransform() const
{
	std::stack<Matrix> parentTransforms;
	auto pParent = GetOwner()->GetParent();

	while (pParent)
	{
		if (auto pParentTransform = pParent->GetComponent<TransformComponent>())
		{
			Matrix parentTransform;

			parentTransform *= pParentTransform->passScale ? Matrix::Scaling(pParentTransform->GetRelativeScale()) : Matrix::Identity();
			parentTransform *= pParentTransform->passRotation ? Matrix::Rotation(pParentTransform->GetRelativeAngle()) : Matrix::Identity();
			parentTransform *= pParentTransform->passTranslation ? Matrix::Translation(pParentTransform->GetRelativePosition()) : Matrix::Identity();

			parentTransforms.push(parentTransform);
		}

		pParent = pParent->GetParent();
	}

	Matrix concatenated;
	while (parentTransforms.empty() == false)
	{
		concatenated *= parentTransforms.top();
		parentTransforms.pop();
	}

	return concatenated;
}

Matrix TransformComponent::GetRelativeTransform() const
{
	return Matrix::Scaling(relativeScale) *
		Matrix::Rotation(relativeAngle) *
		Matrix::Translation(relativePosition);
}

Matrix TransformComponent::GetAbsoluteTransform() const
{
	return GetRelativeTransform() * GetParentAbsoluteTransform();
}

void TransformComponent::Bind()
{
	pConstantBuffer->Update(GetAbsoluteTransform());
	pConstantBuffer->Bind();
}

JSON TransformComponent::ToJson() const
{
	JSON json;

	json["relativePosition"] = relativePosition;
	json["relativeAngle"] = relativeAngle;
	json["relativeScale"] = relativeScale;

	json["passTranslation"] = passTranslation;
	json["passRotation"] = passRotation;
	json["passScale"] = passScale;

	return json;
}

void TransformComponent::FromJson(const JSON& json)
{
	relativePosition = json["relativePosition"];
	relativeAngle = json["relativeAngle"];
	relativeScale = json["relativeScale"];

	passTranslation = json["passTranslation"];
	passRotation = json["passRotation"];
	passScale = json["passScale"];
}

void TransformComponent::DrawWidget()
{
	ImGui::DragFloat3("relative position", &relativePosition[0], 0.01f);
	ImGui::DragFloat3("relative angle", &relativeAngle[0], 0.01f);
	ImGui::DragFloat3("relative scale", &relativeScale[0], 0.01f);

	const auto absolutePosition = GetAbsoluteTransform().Decompose().position;

	ImGui::NewLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("absolute position : [%.3f, %.3f, %.3f]", absolutePosition.x, absolutePosition.y, absolutePosition.z);

	ImGui::NewLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("pass down");
	ImGui::Checkbox("translation", &passTranslation);
	ImGui::Checkbox("rotation", &passRotation);
	ImGui::Checkbox("scale", &passScale);
}