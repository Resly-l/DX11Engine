#include "render_system.h"
#include "rendering/render_target.h"
#include "rendering/depth_stencil.h"
#include "rendering/texture.h"

#include "component_factory.h"
#include "components/transform_component.h"
#include "components/render_component.h"
#include "components/camera_component.h"
#include "components/light_component.h"

#include "input/keyboard.h"
#include "input/mouse.h"

RenderSystem::RenderSystem()
{
	editorCamera.AssignComponent<TransformComponent>();
	editorCamera.AssignComponent<CameraComponent>();
	editorCamera.SetName("editor_camera");

	passResources["framebuffer_rt"] = std::make_shared<RenderTarget>(RenderTarget::Usage::ruRENDER_TARGET);
	passResources["framebuffer_ds"] = std::make_shared<DepthStencil>(DepthStencil::Usage::duDEPTH_STENCIL);

	passResources["light_resource"] = std::make_shared<LightResource>();
}

void RenderSystem::Update(double deltaSeconds)
{
	UpdateEditorCamera(deltaSeconds);
	BindCameraTransform();
	PassLightResource();
	SubmitRenderComponents();

	renderQueue.Render(passResources);
}

void RenderSystem::SetEditorCameraEngagement(bool bEngage)
{
	bEditorCameraEngaged = bEngage;

	if (bEngage && bUseEditorCamera)
	{
		Mouse::SetCursorVisibility(false);
		Mouse::SetCursorMovement(false);
		Mouse::SetConfinement(true);
		Mouse::SetImGuiMouseUsage(false);
	}
	else
	{
		Mouse::SetCursorVisibility(true);
		Mouse::SetCursorMovement(true);
		Mouse::SetConfinement(false);
		Mouse::SetImGuiMouseUsage(true);
	}
}

void RenderSystem::SetSkyboxTexture(std::shared_ptr<TextureCube> pTextureCube)
{
	passResources["skybox_texture"] = pTextureCube;
}

RenderTarget& RenderSystem::GetFramebufferRenderTarget()
{
	return static_cast<RenderTarget&>(*passResources["framebuffer_rt"].get());
}

void RenderSystem::DrawWidget()
{
	ImGui::Checkbox("use editor camera", &bUseEditorCamera);
}

void RenderSystem::BindCameraTransform()
{
	if (bUseEditorCamera)
	{
		editorCamera.GetComponent<CameraComponent>()->Bind();
	}
	else if (pSceneCamera)
	{
		pSceneCamera->Bind();
	}
}

void RenderSystem::PassLightResource()
{
	auto pLightResource = static_pointer_cast<LightResource>(passResources["light_resource"]);
	pLightResource->lightProperties.clear();

	for (auto& pLightComponent : ComponentFactory::GetInstances(LightComponent::ID))
	{
		pLightResource->lightProperties.push_back(static_cast<LightComponent*>(pLightComponent.get())->GetProperty());
	}
}

void RenderSystem::SubmitRenderComponents()
{
	for (auto& pRenderComponent : ComponentFactory::GetInstances(RenderComponent::ID))
	{
		static_cast<RenderComponent*>(pRenderComponent.get())->Submit(renderQueue);
	}
}

void RenderSystem::UpdateEditorCamera(double deltaSeconds)
{
	if (bEditorCameraEngaged == false || bUseEditorCamera == false)
	{
		return;
	}

	auto pTransform = editorCamera.GetComponent<TransformComponent>();

	// rotation
	{
		POINT pt = {};
		for (auto& delta : Mouse::GetRawDeltas())
		{
			pt.x += delta.x;
			pt.y += delta.y;
		}

		static constexpr float fPole = fPI * 0.499999f;
		float fRotationFactor = 1.0f / Renderer::GetSwapChainWidth();
		float fPitchDelta = (float)pt.y * fRotationFactor;
		float fYawDelta = (float)pt.x * fRotationFactor;

		float fPitch = std::clamp(fPitchDelta + pTransform->GetAngle().x, -fPole, fPole);
		float fYaw = fYawDelta + pTransform->GetAngle().y;
		pTransform->SetAngle({ fPitch, fYaw, 0.0f, 0.0f });
	}

	// translation
	{
		const Matrix mRotation = Matrix::Rotation(pTransform->GetAngle());
		const Vector vDirection = Vector(0.0f, 0.0f, 1.0f, 0.0f) * mRotation;
		const Vector vRightside = Vector(1.0f, 0.0f, 0.0f, 0.0f) * mRotation;

		if (Keyboard::IsKeyHold('W')) pTransform->SetPosition(pTransform->GetPosition() + vDirection * (float)deltaSeconds);
		if (Keyboard::IsKeyHold('A')) pTransform->SetPosition(pTransform->GetPosition() - vRightside * (float)deltaSeconds);
		if (Keyboard::IsKeyHold('S')) pTransform->SetPosition(pTransform->GetPosition() - vDirection * (float)deltaSeconds);
		if (Keyboard::IsKeyHold('D')) pTransform->SetPosition(pTransform->GetPosition() + vRightside * (float)deltaSeconds);
	}
}