#pragma once
#include "rendering/render_pass.h"

class SkyboxPass : public RenderPass
{
public:
	SkyboxPass();

public:
	void Render(std::unordered_map<std::string, std::shared_ptr<Resource>>& passResources) override;

private:
	void InitializeShaders();
	void InitializeRenderState();
	void InitializeCube();

private:
	std::unique_ptr<Mesh> pSkyCube;
};