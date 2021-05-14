#include "render_state.h"
#include "renderer.h"
#include "console.h"

RenderState::RenderState()
	:
	topology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
	viewport.Width = (float)Renderer::GetSwapChainWidth();
	viewport.Height = (float)Renderer::GetSwapChainHeight();
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
}

void RenderState::InitializePrimitiveTopology(const D3D11_PRIMITIVE_TOPOLOGY& topology)
{
	this->topology = topology;
}

void RenderState::InitializeViewport(float fWidth, float fHeight)
{
	viewport.Width = fWidth;
	viewport.Height = fHeight;
}

void RenderState::InitializeDepthStencilState(const D3D11_DEPTH_STENCIL_DESC& dsDesc)
{
	if (FAILED(Renderer::GetDevice()->CreateDepthStencilState(&dsDesc, &pDepthStencilState)))
	{
		Console::AddLog({ Log::Type::ltERROR, "render state : failed to create depth stencil state" });
	}
}

void RenderState::InitializeRasterizerState(const D3D11_RASTERIZER_DESC& rasterizerDesc)
{
	if (FAILED(Renderer::GetDevice()->CreateRasterizerState(&rasterizerDesc, &pRasterizerState)))
	{
		Console::AddLog({ Log::Type::ltERROR, "render state : failed to create depth rasterizer state" });
	}
}

void RenderState::InitializeSamplerState(const D3D11_SAMPLER_DESC& samplerDesc, uint32_t uSlot)
{
	ComPtr<ID3D11SamplerState> pSamplerState;
	if (FAILED(Renderer::GetDevice()->CreateSamplerState(&samplerDesc, &pSamplerState)))
	{
		Console::AddLog({ Log::Type::ltERROR, "render state : failed to create sampler state" });
		return;
	}

	samplerStatePtrs.push_back({ std::move(pSamplerState), uSlot });
}

void RenderState::InitializeBlendState(const D3D11_BLEND_DESC& blendDesc, const std::optional<Vector>& vBlendFactor)
{
	if (FAILED(Renderer::GetDevice()->CreateBlendState(&blendDesc, &pBlendState)))
	{
		Console::AddLog({ Log::Type::ltERROR, "render state : failed to create blend state" });
	}
	this->vBlendFactor = vBlendFactor;
}

void RenderState::Bind() const
{
	Renderer::GetContext()->IASetPrimitiveTopology(topology);

	Renderer::GetContext()->RSSetViewports(1, &viewport);

	if (pDepthStencilState) Renderer::GetContext()->OMSetDepthStencilState(pDepthStencilState.Get(), 0xFF);
	if (pRasterizerState) Renderer::GetContext()->RSSetState(pRasterizerState.Get());
	for (auto& [pSamplerState, uSlot] : samplerStatePtrs)
	{
		if (pSamplerState) Renderer::GetContext()->PSSetSamplers(uSlot, 1, pSamplerState.GetAddressOf());
	}
	if (pBlendState) Renderer::GetContext()->OMSetBlendState(pBlendState.Get(), vBlendFactor ? &(*vBlendFactor)[0] : nullptr, 0xFFFFFFFF);
}