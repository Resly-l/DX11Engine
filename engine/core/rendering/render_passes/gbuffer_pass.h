#pragma once
#include "rendering/render_pass.h"

class RenderTarget;

class GBufferPass : public RenderPass
{
private:
	std::shared_ptr<RenderTarget> pPositionGBuffer;
	std::shared_ptr<RenderTarget> pNormalGBuffer;
	std::shared_ptr<RenderTarget> pAlbedoGBuffer;
	std::shared_ptr<RenderTarget> pSpecularGBuffer;

public:
	GBufferPass();

public:
	void Render(std::unordered_map<std::string, std::shared_ptr<Resource>>& passResources) override;

private:
	void InitializeShaders();
	void InitializeRenderState();
};