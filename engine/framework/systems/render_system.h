#pragma once
#include "system.h"
#include "entity.h"
#include "rendering/render_queue.h"

class TextureCube;
class CameraComponent;
class RenderTarget;

class RenderSystem : public System<RenderSystem>
{
private:
	CameraComponent* pSceneCamera = nullptr;
	Entity editorCamera;
	bool bUseEditorCamera = false;
	bool bEditorCameraEngaged = false;

	std::unordered_map<std::string, std::shared_ptr<Resource>> passResources;
	RenderQueue renderQueue;

public:
	RenderSystem();

public:
	void Update(double deltaSeconds) override;

	void SetCamera(CameraComponent* pCamera) { pSceneCamera = pCamera; }
	void SetEditorCameraUsage(bool bUse) { bUseEditorCamera = bUse; }
	void SetEditorCameraEngagement(bool bEngage);
	void SetSkyboxTexture(std::shared_ptr<TextureCube> pTextureCube);

	Entity& GetEditorCamera() { return editorCamera; }
	RenderTarget& GetFramebufferRenderTarget();

	void DrawWidget() override;

private:
	void BindCameraTransform();
	void PassLightResource();
	void SubmitRenderComponents();
	void UpdateEditorCamera(double deltaSeconds);
};