#include "camera_component.h"
#include "entity.h"
#include "transform_component.h"
#include "rendering/renderer.h"

CameraComponent::CameraComponent()
	:
	viewProjectionCB(ConstantType::ctCAMERA_TRANSFORM, ShaderBindFlag::btBIND_ALL, ViewProjection{})
{}

Matrix CameraComponent::GetViewMatrix() const
{
	Vector position(0.0f, 0.0f, 0.0f, 0.0f);
	Vector vDirection(0.0f, 0.0f, 1.0f, 0.0f);
	Vector vUpside(0.0f, 1.0f, 0.0f, 0.0f);

	if (auto pTransformComponent = GetOwner()->GetComponent<TransformComponent>())
	{
		position = pTransformComponent->GetPosition();

		const auto transform = Matrix::Rotation(pTransformComponent->GetAngle());

		vDirection *= transform;
		vUpside *= transform;
	}

	return Matrix::LookTo(position, vDirection, vUpside);
}

Matrix CameraComponent::GetProjectionMatrix() const
{
	if (aspectRatio == 0.0f)
	{
		const float fSwapChainAspectRatio = (float)Renderer::GetSwapChainWidth() / (float)Renderer::GetSwapChainHeight();

		return Matrix::PerspectiveFov(horizontalFOV, fSwapChainAspectRatio, nearPlaneDistance, farPlaneDistance);
	}
	return Matrix::PerspectiveFov(horizontalFOV, aspectRatio, nearPlaneDistance, farPlaneDistance);
}

void CameraComponent::Bind()
{
	viewProjectionCB.Update(ViewProjection{ GetViewMatrix(), GetProjectionMatrix() });
	viewProjectionCB.Bind();
}

JSON CameraComponent::ToJson() const
{
	JSON json;

	json["horizontalFOV"] = horizontalFOV;
	json["nearPlaneDistance"] = nearPlaneDistance;
	json["farPlaneDistance"] = farPlaneDistance;
	json["aspectRatio"] = aspectRatio;

	return json;
}

void CameraComponent::FromJson(const JSON& json)
{
	horizontalFOV = json["horizontalFOV"];
	nearPlaneDistance = json["nearPlaneDistance"];
	farPlaneDistance = json["farPlaneDistance"];
	aspectRatio = json["aspectRatio"];
}

void CameraComponent::DrawWidget()
{
	ImGui::DragFloat("horizontal fov", &horizontalFOV, 0.01f);
	ImGui::DragFloat("near plane distance", &nearPlaneDistance, 0.01f);
	ImGui::DragFloat("far plane distance", &farPlaneDistance, 0.01f);
	ImGui::InputFloat("aspect ratio", &aspectRatio);
}