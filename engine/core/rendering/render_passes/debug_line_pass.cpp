#include "debug_line_pass.h"
#include "rendering/render_target.h"
#include "rendering/depth_stencil.h"
#include "rendering/drawable.h"

DebugLinePass::DebugLinePass()
	:
	RenderPass(RenderPass::Tag::ptDEBUG_LINE),
	colorCB(ConstantType::ctMATERIAL, ShaderBindFlag::bfBIND_PS, float4{})
{
	InitializeShaders();
	InitializeRenderState();
}

void DebugLinePass::Render(std::unordered_map<std::string, std::shared_ptr<Resource>>& passResources)
{
	auto pFrameBufferRT = static_cast<RenderTarget*>(passResources["framebuffer_rt"].get());
	auto pFrameBufferDS = static_cast<DepthStencil*>(passResources["framebuffer_ds"].get());

	pFrameBufferRT->BindAsTarget(pFrameBufferDS);

	RenderVisibleLines();
	RenderInvisibleLines();
}

void DebugLinePass::InitializeShaders()
{
	shader.InitializeVertexShader("../asset/shaders/SolidVS.cso");
	shader.InitializePixelShader("../asset/shaders/SolidPS.cso");
}

void DebugLinePass::InitializeRenderState()
{
	renderState.InitializePrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.StencilEnable = FALSE;
	renderState.InitializeDepthStencilState(dsDesc);

	renderState.InitializeBlendState(CD3D11_BLEND_DESC{ CD3D11_DEFAULT{} });

	D3D11_RASTERIZER_DESC rasterizerDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.AntialiasedLineEnable = TRUE;
	renderState.InitializeRasterizerState(rasterizerDesc);
}

void DebugLinePass::RenderVisibleLines()
{
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	renderState.InitializeDepthStencilState(dsDesc);
	renderState.Bind();

	colorCB.Update(float4(0.0f, 1.0f, 1.0f, 1.0f));
	colorCB.Bind();

	for (auto& pDrawable : drawablePtrs)
	{
		pDrawable->Draw();
	}
}

void DebugLinePass::RenderInvisibleLines()
{
	dsDesc.DepthFunc = D3D11_COMPARISON_GREATER;
	renderState.InitializeDepthStencilState(dsDesc);
	renderState.Bind();

	colorCB.Update(float4(0.0f, 0.6f, 0.6f, 1.0f));
	colorCB.Bind();

	for (auto& pDrawable : drawablePtrs)
	{
		pDrawable->Draw();
	}
}