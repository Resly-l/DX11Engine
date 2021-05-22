#include "render_system.h"
#include "rendering/render_target.h"
#include "rendering/depth_stencil.h"
#include "rendering/texture.h"
#include "rendering/mesh.h"

#include "component_factory.h"
#include "components/transform_component.h"
#include "components/model_component.h"
#include "components/camera_component.h"
#include "components/light_component.h"
#include "components/physics_component.h"

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

void RenderSystem::Update(double deltaSeconds, bool simulate)
{
	UpdateEditorCamera(deltaSeconds);
	BindCamera();
	PassLightResource();
	SubmitRenderComponents();

	if (drawDebugLines)
	{
		SubmitColliderOutlines();
	}

	renderQueue.Render(passResources);
	debugLinePtrs.clear();
}

void RenderSystem::SetEditorCameraEngagement(bool engage)
{
	editorCameraEngaged = engage;

	if (engage && useEditorCamera)
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
	ImGui::Checkbox("draw debug lines", &drawDebugLines);

	ImGui::NewLine();
	ImGui::Checkbox("use editor camera", &useEditorCamera);

	ImGui::NewLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("editor camera transform");
	editorCamera.GetComponent<TransformComponent>()->DrawWidget();
}

void RenderSystem::BindCamera()
{
	if (useEditorCamera)
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
	for (auto& pComponent : ComponentFactory::GetInstances(ModelComponent::ID))
	{
		auto pModelComponent = static_cast<ModelComponent*>(pComponent.get());

		if (pModelComponent->visible)
		{
			renderQueue.QueueDrawable(pModelComponent->transparent ? RenderPass::Tag::ptALPHA : RenderPass::Tag::ptGBUFFER, pModelComponent);
		}

		if (pModelComponent->castShadow)
		{
			renderQueue.QueueDrawable(RenderPass::Tag::ptSHADOW, pModelComponent);
		}
	}
}

void RenderSystem::SubmitColliderOutlines()
{
	class DebugLine : public Drawable
	{
	private:
		Mesh lineMesh;
		ConstantBuffer transformCB;

	public:
		DebugLine(Mesh mesh, const Matrix& transform)
			:
			lineMesh(std::move(mesh)),
			transformCB(ConstantType::ctINSTANCE_TRANSFORM, ShaderBindFlag::bfBIND_VS, transform)
		{}

	public:
		void Draw() override
		{
			transformCB.Bind();
			lineMesh.Draw();
		}
	};

	for (auto& pPhysicsComponent : ComponentFactory::GetInstances(PhysicsComponent::ID))
	{
		if (auto pTransformComponent = pPhysicsComponent->GetOwner()->GetComponent<TransformComponent>())
		{
			if (auto pCollider = static_cast<PhysicsComponent*>(pPhysicsComponent.get())->GetCollider())
			{
				if (auto pBox = dynamic_cast<BoundingBox*>(pCollider))
				{
					debugLinePtrs.push_back(std::make_unique<DebugLine>(MakeBoxMesh(pBox->minExtent, pBox->maxExtent), pTransformComponent->GetTransformMatrix()));
				}
				else if (auto pSphere = dynamic_cast<BoundingSphere*>(pCollider))
				{
					debugLinePtrs.push_back(std::make_unique<DebugLine>(MakeSphereMesh(pSphere->center, pSphere->radius), pTransformComponent->GetTransformMatrix()));
				}

				renderQueue.QueueDrawable(RenderPass::Tag::ptDEBUG_LINE, debugLinePtrs.back().get());
			}
		}
	}
}

void RenderSystem::UpdateEditorCamera(double deltaSeconds)
{
	if (editorCameraEngaged == false || useEditorCamera == false)
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

		static constexpr float fPole = (float)pi * 0.499999f;
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

Mesh RenderSystem::MakeBoxMesh(Vector minExtent, Vector maxExtent)
{
	std::vector<Vertex> vertices(8);
	vertices[0].position = { minExtent.x, maxExtent.y, minExtent.z };
	vertices[1].position = { maxExtent.x, maxExtent.y, minExtent.z };
	vertices[2].position = { maxExtent.x, minExtent.y, minExtent.z };
	vertices[3].position = { minExtent.x, minExtent.y, minExtent.z };
	vertices[4].position = { minExtent.x, maxExtent.y, maxExtent.z };
	vertices[5].position = { maxExtent.x, maxExtent.y, maxExtent.z };
	vertices[6].position = { maxExtent.x, minExtent.y, maxExtent.z };
	vertices[7].position = { minExtent.x, minExtent.y, maxExtent.z };

	std::vector<uint32_t> indices = {
		0,1,1,2,2,3,3,0,
		4,5,5,6,6,7,7,4,
		0,4,1,5,2,6,3,7
	};

	return Mesh(std::move(vertices), std::move(indices));
}

Mesh RenderSystem::MakeSphereMesh(Vector center, float radius)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	return Mesh(std::move(vertices), std::move(indices));
}