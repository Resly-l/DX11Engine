#pragma once
#include "rendering/render_pass.h"
#include "rendering/constant_buffer.h"
#include "rendering/shadow_cubemap.h"

class ShadowPass : public RenderPass
{
public:
	ShadowPass();

public:
	void Render(std::unordered_map<std::string, std::shared_ptr<Resource>>& passResources) override;

private:
	void InitializeShaders();
	void InitializeRenderStates();

private:
	std::vector<ShadowCubemap> shadowMaps;
	ConstantBuffer lightTransformCB;

	// lhs = direction, rhs = upside
	std::pair<Vector, Vector> faceDirections[6];
	Matrix projection;
};