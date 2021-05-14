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
	Vector vPosition(0.0f, 0.0f, 0.0f, 0.0f);
	Vector vDirection(0.0f, 0.0f, 1.0f, 0.0f);
	Vector vUpside(0.0f, 1.0f, 0.0f, 0.0f);

	if (auto pTransformComponent = pOwner->GetComponent<TransformComponent>())
	{
		vPosition = pTransformComponent->GetPosition();

		const auto mTransform = Matrix::Rotation(pTransformComponent->GetAngle());

		vDirection *= mTransform;
		vUpside *= mTransform;
	}

	return Matrix::LookTo(vPosition, vDirection, vUpside);
}

Matrix CameraComponent::GetProjectionMatrix() const
{
	if (fAspectRatio == 0.0f)
	{
		const float fSwapChainAspectRatio = (float)Renderer::GetSwapChainWidth() / (float)Renderer::GetSwapChainHeight();

		return Matrix::PerspectiveFov(fHorizontalFOV, fSwapChainAspectRatio, fNearPlaneDistance, fFarPlaneDistance);
	}
	return Matrix::PerspectiveFov(fHorizontalFOV, fAspectRatio, fNearPlaneDistance, fFarPlaneDistance);
}

void CameraComponent::Bind()
{
	viewProjectionCB.Update(ViewProjection{ GetViewMatrix(), GetProjectionMatrix() });
	viewProjectionCB.Bind();
}

JSON CameraComponent::ToJson() const
{
	JSON json;

	json["fHorizontalFOV"] = fHorizontalFOV;
	json["fNearPlaneDistance"] = fNearPlaneDistance;
	json["fFarPlaneDistance"] = fFarPlaneDistance;
	json["fAspectRatio"] = fAspectRatio;

	return json;
}

void CameraComponent::FromJson(const JSON& json)
{
	fHorizontalFOV = json["fHorizontalFOV"];
	fNearPlaneDistance = json["fNearPlaneDistance"];
	fFarPlaneDistance = json["fFarPlaneDistance"];
	fAspectRatio = json["fAspectRatio"];
}

void CameraComponent::DrawWidget()
{
	ImGui::DragFloat("horizontal fov", &fHorizontalFOV, 0.01f);
	ImGui::DragFloat("near plane distance", &fNearPlaneDistance, 0.01f);
	ImGui::DragFloat("far plane distance", &fFarPlaneDistance, 0.01f);
	ImGui::InputFloat("aspect ratio", &fAspectRatio);
}