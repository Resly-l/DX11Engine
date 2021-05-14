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

private:
	Usage usage;

	ComPtr<ID3D11RenderTargetView> pRenderTargetView;
	ComPtr<ID3D11ShaderResourceView> pShaderResourceView;

	uint32_t uWidth;
	uint32_t uHeight;

	// synchronize resolution with backbuffer
	bool bSyncResolution;

public:
	RenderTarget(Usage usage);
	RenderTarget(Usage usage, uint32_t uWidth, uint32_t uHeight);

public:
	void BindAsTarget(DepthStencil* pDepthStencil);
	void BindAsTexture(uint32_t uSlot);
	void Clear(const Vector& vClearColor = { 0.0f, 0.0f, 0.0f, 1.0f });

	ID3D11RenderTargetView* GetView();
	ID3D11ShaderResourceView* GetShaderResourceView() const { return pShaderResourceView.Get(); }

private:
	void Initialize();

	static DXGI_FORMAT MapFormat(Usage usage);
};