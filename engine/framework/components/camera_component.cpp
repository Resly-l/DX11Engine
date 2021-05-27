#include "camera_component.h"
#include "entity.h"
#include "transform_component.h"
#include "rendering/constant_buffer.h"

std::unique_ptr<ConstantBuffer> CameraComponent::pViewProjectionCB;

CameraComponent::CameraComponent()
{
	// delaying initialization to be initialized after renderer
	if (pViewProjectionCB == nullptr)
	{
		pViewProjectionCB = std::make_unique<ConstantBuffer>(ConstantType::ctCAMERA_TRANSFORM, ShaderBindFlag::btBIND_ALL, ViewProjection{});
	}
}

Matrix CameraComponent::GetViewMatrix() const
{
	Vector position(0.0f, 0.0f, 0.0f, 0.0f);
	Vector vDirection(0.0f, 0.0f, 1.0f, 0.0f);
	Vector vUpside(0.0f, 1.0f, 0.0f, 0.0f);

	if (auto pTransformComponent = GetOwner()->GetComponent<TransformComponent>())
	{
		const auto absoluteTransform = pTransformComponent->GetAbsoluteTransform();
		const auto decomposed = absoluteTransform.Decompose();
		const auto rotation = Matrix::Rotation(decomposed.angle);

		position = decomposed.position;
		vDirection *= rotation;
		vUpside *= rotation;
	}

	return Matrix::LookTo(position, vDirection, vUpside);
}

Matrix CameraComponent::GetProjectionMatrix() const
{
	float finalAspectRatio = aspectRatio;

	// 0.0f = use swap chain aspect ratio
	if (aspectRatio == 0.0f)
	{
		finalAspectRatio = (float)Renderer::GetSwapChainWidth() / (float)Renderer::GetSwapChainHeight();
	}

	return Matrix::PerspectiveFov(horizontalFOV, finalAspectRatio, nearPlaneDistance, farPlaneDistance);
}

void CameraComponent::Bind()
{
	pViewProjectionCB->Update(ViewProjection{ GetViewMatrix(), GetProjectionMatrix() });
	pViewProjectionCB->Bind();
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