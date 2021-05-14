#pragma once
#include "common.h"

class RenderState
{
private:
	D3D11_PRIMITIVE_TOPOLOGY topology;

	D3D11_VIEWPORT viewport;

	ComPtr<ID3D11DepthStencilState> pDepthStencilState;
	ComPtr<ID3D11RasterizerState> pRasterizerState;
	// rhs = bind slot
	std::vector<std::pair<ComPtr<ID3D11SamplerState>, uint32_t>> samplerStatePtrs;
	ComPtr<ID3D11BlendState> pBlendState;

	std::optional<Vector> vBlendFactor;

public:
	RenderState();

public:
	void InitializePrimitiveTopology(const D3D11_PRIMITIVE_TOPOLOGY& topology);

	void InitializeViewport(float fWidth, float fHeight);

	void InitializeDepthStencilState(const D3D11_DEPTH_STENCIL_DESC& dsDesc);
	void InitializeRasterizerState(const D3D11_RASTERIZER_DESC& rasterizerDesc);
	void InitializeSamplerState(const D3D11_SAMPLER_DESC& samplerDesc, uint32_t uSlot);
	void InitializeBlendState(const D3D11_BLEND_DESC& blendDesc, const std::optional<Vector>& vBlendFactor = {});

	void Bind() const;
};