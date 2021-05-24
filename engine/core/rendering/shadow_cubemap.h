#pragma once
#include "depth_stencil.h"

class ShadowCubemap
{
public:
	ShadowCubemap(uint32_t resulution = 1024);

public:
	ID3D11ShaderResourceView* GetShaderResourceView() const { return pShaderResourceView.Get(); };
	DepthStencil& GetDepthBuffer(uint32_t faceIndex) { return *depthBufferPtrs[faceIndex]; };

private:
	ComPtr<ID3D11ShaderResourceView> pShaderResourceView;
	std::array<std::unique_ptr<DepthStencil>, 6> depthBufferPtrs;
};