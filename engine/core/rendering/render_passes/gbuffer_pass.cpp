#include "gbuffer_pass.h"
#include "rendering/renderer.h"
#include "rendering/render_target.h"
#include "rendering/depth_stencil.h"
#include "rendering/mesh.h"
#include "rendering/drawable.h"

GBufferPass::GBufferPass()
	:
	RenderPass(RenderPass::Tag::ptGBUFFER)
{
	InitializeShaders();
	InitializeRenderState();

	pPositionGBuffer = std::make_shared<RenderTarget>(RenderTarget::Usage::ruGBUFFER);
	pNormalGBuffer = std::make_shared<RenderTarget>(RenderTarget::Usage::ruGBUFFER);
	pAlbedoGBuffer = std::make_shared<RenderTarget>(RenderTarget::Usage::ruGBUFFER);
	pSpecularGBuffer = std::make_shared<RenderTarget>(RenderTarget::Usage::ruGBUFFER);
}

void GBufferPass::Render(std::unordered_map<std::string, std::shared_ptr<Resource>>& passResources)
{
	auto pFrameBufferDS = static_cast<DepthStencil*>(passResources["framebuffer_ds"].get());

	pFrameBufferDS->Clear();

	pPositionGBuffer->Clear();
	pNormalGBuffer->Clear();
	pAlbedoGBuffer->Clear();
	pSpecularGBuffer->Clear();

	ID3D11ShaderResourceView* pNullSRV = nullptr;
	for (uint32_t slot = 0; slot <= 3; slot++)
	{
		Renderer::GetContext()->PSSetShaderResources(slot, 1, &pNullSRV);
	}

	ID3D11RenderTargetView* const renderTargetPtrs[] = {
		pPositionGBuffer->GetView(),
		pNormalGBuffer->GetView(),
		pAlbedoGBuffer->GetView(),
		pSpecularGBuffer->GetView()
	};

	Renderer::GetContext()->OMSetRenderTargets(4, renderTargetPtrs, pFrameBufferDS->GetView());

	for (auto pDrawable : drawablePtrs)
	{
		pDrawable->Draw();
	}

	passResources["gbuffer_position"] = pPositionGBuffer;
	passResources["gbuffer_normal"] = pNormalGBuffer;
	passResources["gbuffer_albedo"] = pAlbedoGBuffer;
	passResources["gbuffer_specular"] = pSpecularGBuffer;
}

void GBufferPass::InitializeShaders()
{
	shader.InitializeVertexShader("../asset/shaders/GBufferVS.cso");
	shader.InitializePixelShader("../asset/shaders/GBufferPS.cso");
}

void GBufferPass::InitializeRenderState()
{
	renderState.InitializePrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.StencilEnable = FALSE;
	renderState.InitializeDepthStencilState(dsDesc);

	D3D11_BLEND_DESC blendDesc = CD3D11_BLEND_DESC{ CD3D11_DEFAULT{} };
	blendDesc.RenderTarget->BlendEnable = FALSE;
	renderState.InitializeBlendState(blendDesc);

	D3D11_RASTERIZER_DESC rasterizerDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	renderState.InitializeRasterizerState(rasterizerDesc);

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.MipLODBias = 0.0f;
	renderState.InitializeSamplerState(samplerDesc, 0);
}