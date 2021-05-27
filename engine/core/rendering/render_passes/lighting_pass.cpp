#include "lighting_pass.h"
#include "rendering/renderer.h"
#include "rendering/mesh.h"
#include "rendering/render_target.h"

#include "components/light_component.h"

LightingPass::LightingPass()
	:
	RenderPass(RenderPass::Tag::ptLIGHTING),
	lightCB(ConstantType::ctLIGHT, ShaderBindFlag::bfBIND_PS, LightComponent::Property{}, uMaxLights),
	lightCountCB(ConstantType::ctLIGHT_COUNT, ShaderBindFlag::bfBIND_PS, uint4{})
{
	InitializeShaders();
	InitializeRenderState();
	InitializeScreenPlane();
}

void LightingPass::Render(std::unordered_map<std::string, std::shared_ptr<Resource>>& passResources)
{
	auto pLightResource = static_cast<LightResource*>(passResources["light_resource"].get());

	lightCB.Update(pLightResource->lightProperties);
	lightCB.Bind();
	lightCountCB.Update(uint4{ (uint32_t)pLightResource->lightProperties.size(), 0, 0, 0 });
	lightCountCB.Bind();

	auto pFrameBufferRT = static_cast<RenderTarget*>(passResources["framebuffer_rt"].get());

	pFrameBufferRT->Clear();
	pFrameBufferRT->BindAsTarget(nullptr);

	static_cast<RenderTarget*>(passResources["gbuffer_position"].get())->BindAsTexture(0);
	static_cast<RenderTarget*>(passResources["gbuffer_normal"].get())->BindAsTexture(1);
	static_cast<RenderTarget*>(passResources["gbuffer_albedo"].get())->BindAsTexture(2);
	static_cast<RenderTarget*>(passResources["gbuffer_specular"].get())->BindAsTexture(3);

	pScreenPlane->Draw();
}

void LightingPass::InitializeShaders()
{
	shader.InitializeVertexShader("../asset/shaders/FullscreenVS.cso");
	shader.InitializePixelShader("../asset/shaders/LightingPS.cso");
}

void LightingPass::InitializeRenderState()
{
	renderState.InitializePrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = FALSE;
	dsDesc.StencilEnable = FALSE;
	renderState.InitializeDepthStencilState(dsDesc);

	renderState.InitializeBlendState(CD3D11_BLEND_DESC{ CD3D11_DEFAULT{} });

	renderState.InitializeRasterizerState(CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{}));

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.MipLODBias = 0.0f;
	renderState.InitializeSamplerState(samplerDesc, 0);

	samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	renderState.InitializeSamplerState(samplerDesc, 1);
}

void LightingPass::InitializeScreenPlane()
{
	std::vector<Vertex> vertices(4);
	vertices[0].position = { -1.0f, 1.0f, 0.0f };
	vertices[1].position = { 1.0f, 1.0f, 0.0f };
	vertices[2].position = { 1.0f, -1.0f, 0.0f };
	vertices[3].position = { -1.0f, -1.0f, 0.0f };

	std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };

	pScreenPlane = std::make_unique<Mesh>(std::move(vertices), std::move(indices));
}