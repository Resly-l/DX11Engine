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
	bool useEditorCamera = false;
	bool editorCameraEngaged = false;
	
	std::vector<std::unique_ptr<Drawable>> debugLinePtrs;
	bool drawDebugLines = false;

	std::unordered_map<std::string, std::shared_ptr<Resource>> passResources;
	RenderQueue renderQueue;

public:
	RenderSystem();

public:
	void Update(double deltaSeconds, bool simulate) override;

	void SetCamera(CameraComponent* pCamera) { pSceneCamera = pCamera; }
	void SetEditorCameraUsage(bool useEditorCamera) { this->useEditorCamera = useEditorCamera; }
	void SetEditorCameraEngagement(bool engage);
	void SetSkyboxTexture(std::shared_ptr<TextureCube> pTextureCube);

	RenderTarget& GetFramebufferRenderTarget();

	void DrawWidget() override;

private:
	void BindCamera();
	void PassLightResource();
	void AnimateAnimations(double deltaSeconds);
	void SubmitRenderComponents();
	void SubmitColliderOutlines();
	void UpdateEditorCamera(double deltaSeconds);

	static Mesh MakeBoxMesh(Vector minExtent, Vector maxExtent);
	static Mesh MakeSphereMesh(Vector center, float radius);
};