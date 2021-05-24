#pragma once
#include "rendering/render_pass.h"
#include "rendering/constant_buffer.h"

class DebugLinePass : public RenderPass
{
public:
	DebugLinePass();

public:
	void Render(std::unordered_map<std::string, std::shared_ptr<Resource>>& passResources) override;

private:
	void InitializeShaders();
	void InitializeRenderState();

	void RenderVisibleLines();
	void RenderInvisibleLines();

private:
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ConstantBuffer colorCB;
};