#include "skybox_pass.h"
#include "rendering/render_target.h"
#include "rendering/depth_stencil.h"
#include "rendering/mesh.h"
#include "rendering/texture.h"
#include "rendering/renderer.h"

SkyboxPass::SkyboxPass()
	:
	RenderPass(RenderPass::Tag::ptSKY)
{
	InitializeShaders();
	InitializeRenderState();
	InitializeCube();
}

void SkyboxPass::Render(std::unordered_map<std::string, std::shared_ptr<Resource>>& passResources)
{
	auto pFrameBufferRT = static_cast<RenderTarget*>(passResources["framebuffer_rt"].get());
	auto pFrameBufferDS = static_cast<DepthStencil*>(passResources["framebuffer_ds"].get());

	pFrameBufferRT->BindAsTarget(pFrameBufferDS);

	if (auto& pSkyboxTexture = passResources["skybox_texture"])
	{
		static_cast<TextureCube*>(pSkyboxTexture.get())->Bind();
		pSkyCube->Draw();
	}
}

void SkyboxPass::InitializeShaders()
{
	shader.InitializeVertexShader("../asset/shaders/SkyboxVS.cso");
	shader.InitializePixelShader("../asset/shaders/SkyboxPS.cso");
}

void SkyboxPass::InitializeRenderState()
{
	renderState.InitializePrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.StencilEnable = FALSE;
	renderState.InitializeDepthStencilState(dsDesc);

	D3D11_BLEND_DESC blendDesc = CD3D11_BLEND_DESC{ CD3D11_DEFAULT{} };
	blendDesc.RenderTarget->BlendEnable = FALSE;
	renderState.InitializeBlendState(blendDesc);

	D3D11_RASTERIZER_DESC rasterizerDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	renderState.InitializeRasterizerState(rasterizerDesc);

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.MipLODBias = 0.0f;
	renderState.InitializeSamplerState(samplerDesc, 0);
}

void SkyboxPass::InitializeCube()
{
	std::vector<Vertex> vertices(8);
	vertices[0].position = { -1.0f, 1.0f, -1.0f };
	vertices[1].position = { 1.0f, 1.0f, -1.0f };
	vertices[2].position = { 1.0f, -1.0f, -1.0f };
	vertices[3].position = { -1.0f, -1.0f, -1.0f };
	vertices[4].position = { -1.0f, 1.0f, 1.0f };
	vertices[5].position = { 1.0f, 1.0f, 1.0f };
	vertices[6].position = { 1.0f, -1.0f, 1.0f };
	vertices[7].position = { -1.0f, -1.0f, 1.0f };

	std::vector<uint32_t> indices = {
		0,1,2, 2,3,0,
		4,0,3, 3,7,4,
		5,4,7, 7,6,5,
		1,5,6, 6,2,1,
		4,5,1, 1,0,4,
		6,7,3, 3,2,6
	};

	pSkyCube = std::make_unique<Mesh>(std::move(vertices), std::move(indices));
}