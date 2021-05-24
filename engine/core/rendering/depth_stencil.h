#pragma once
#include "resource.h"

class DepthStencil : public Resource
{
public:
	enum class Usage
	{
		duDEPTH_STENCIL,
		duSHADOW_DEPTH
	};

public:
	DepthStencil(Usage usage);
	DepthStencil(Usage usage, uint32_t width, uint32_t height);
	DepthStencil(ComPtr<ID3D11Texture2D> pTexture, uint32_t faceIndex);

public:
	ID3D11DepthStencilView* GetView();
	void BindAsTexture(uint32_t slot);
	void BindAsTarget();
	void Clear();

private:
	void Initialize();

	static DXGI_FORMAT MapTextureFormat(Usage usage);
	static DXGI_FORMAT MapDepthStencilFormat(Usage usage);
	static DXGI_FORMAT MapShaderResourceFormat(Usage usage);

private:
	Usage usage;

	ComPtr<ID3D11DepthStencilView> pDepthStencilView;
	ComPtr<ID3D11ShaderResourceView> pShaderResourceView;

	uint32_t width;
	uint32_t height;

	// synchronize resolution with backbuffer
	bool syncResolution;
};