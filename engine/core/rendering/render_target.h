#pragma once
#include "resource.h"

class DepthStencil;

class RenderTarget : public Resource
{
public:
	enum class Usage
	{
		ruRENDER_TARGET,
		ruGBUFFER
	};

public:
	RenderTarget(Usage usage);
	RenderTarget(Usage usage, uint32_t width, uint32_t height);

public:
	void BindAsTarget(DepthStencil* pDepthStencil);
	void BindAsTexture(uint32_t slot);
	void Clear(const Vector& clearColor = { 0.0f, 0.0f, 0.0f, 1.0f });

	ID3D11RenderTargetView* GetView();
	ID3D11ShaderResourceView* GetShaderResourceView() const { return pShaderResourceView.Get(); }

private:
	void Initialize();

	static DXGI_FORMAT MapFormat(Usage usage);

private:
	Usage usage;

	ComPtr<ID3D11RenderTargetView> pRenderTargetView;
	ComPtr<ID3D11ShaderResourceView> pShaderResourceView;

	uint32_t width;
	uint32_t height;

	// synchronize resolution with backbuffer
	bool syncResolution;
};