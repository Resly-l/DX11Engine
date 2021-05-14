#include "depth_stencil.h"
#include "renderer.h"
#include "console.h"

DepthStencil::DepthStencil(Usage usage)
	:
	usage(usage),
	uWidth(Renderer::GetSwapChainWidth()),
	uHeight(Renderer::GetSwapChainHeight()),
	bSyncResolution(true)
{
	Initialize();
}

DepthStencil::DepthStencil(Usage usage, uint32_t uWidth, uint32_t uHeight)
	:
	usage(usage),
	uWidth(uWidth),
	uHeight(uHeight),
	bSyncResolution(false)
{
	Initialize();
}

DepthStencil::DepthStencil(ComPtr<ID3D11Texture2D> pTexture, uint32_t uFaceIndex)
{
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	dsvDesc.Texture2DArray.ArraySize = 1u;
	dsvDesc.Texture2DArray.FirstArraySlice = uFaceIndex;
	dsvDesc.Texture2DArray.MipSlice = 0u;

	if (FAILED(Renderer::GetDevice()->CreateDepthStencilView(pTexture.Get(), &dsvDesc, &pDepthStencilView)))
	{
		Console::AddLog({ Log::Type::ltERROR, "failed to create depth stencil cubemap" });
	}
}

ID3D11DepthStencilView* DepthStencil::GetView()
{
	if (bSyncResolution)
	{
		const uint32_t uSwapChainWidth = Renderer::GetSwapChainWidth();
		const uint32_t uSwapChainHeight = Renderer::GetSwapChainHeight();

		if (uWidth != uSwapChainWidth || uHeight != uSwapChainHeight)
		{
			uWidth = uSwapChainWidth;
			uHeight = uSwapChainHeight;

			Initialize();
		}
	}

	return pDepthStencilView.Get();
}

void DepthStencil::BindAsTexture(uint32_t uSlot)
{
	Renderer::GetContext()->PSSetShaderResources(uSlot, 1, pShaderResourceView.GetAddressOf());
}

void DepthStencil::BindAsTarget()
{
	Renderer::GetContext()->OMSetRenderTargets(0, nullptr, pDepthStencilView.Get());
}

void DepthStencil::Clear()
{
	Renderer::GetContext()->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DepthStencil::Initialize()
{
	ComPtr<ID3D11Texture2D> pTexture2D;
	{
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = uWidth;
		textureDesc.Height = uHeight;
		textureDesc.ArraySize = 1;
		textureDesc.Format = MapTextureFormat(usage);
		textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.MiscFlags = 0;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MipLevels = 1;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;

		if (FAILED(Renderer::GetDevice()->CreateTexture2D(&textureDesc, nullptr, &pTexture2D)))
		{
			Console::AddLog({ Log::Type::ltERROR, "failed to create depth stencil texture" });
			return;
		}
	}
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = MapDepthStencilFormat(usage);
		dsvDesc.Texture2D.MipSlice = 0;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		if (FAILED(Renderer::GetDevice()->CreateDepthStencilView(pTexture2D.Get(), &dsvDesc, &pDepthStencilView)))
		{
			Console::AddLog({ Log::Type::ltERROR, "failed to create depth stencil view" });
			return;
		}
	}
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = MapShaderResourceFormat(usage);
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0u;
		srvDesc.Texture2D.MipLevels = 1u;

		if (FAILED(Renderer::GetDevice()->CreateShaderResourceView(pTexture2D.Get(), &srvDesc, &pShaderResourceView)))
		{
			Console::AddLog({ Log::Type::ltERROR, "failed to create depth stencil srv" });
			return;
		}
	}
}

DXGI_FORMAT DepthStencil::MapTextureFormat(Usage usage)
{
	switch (usage)
	{
	case DepthStencil::Usage::duDEPTH_STENCIL:
		return DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS;
	case DepthStencil::Usage::duSHADOW_DEPTH:
		return DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
	}

	return DXGI_FORMAT_UNKNOWN;
}

DXGI_FORMAT DepthStencil::MapDepthStencilFormat(Usage usage)
{
	switch (usage)
	{
	case DepthStencil::Usage::duDEPTH_STENCIL:
		return DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
	case DepthStencil::Usage::duSHADOW_DEPTH:
		return DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	}

	return DXGI_FORMAT_UNKNOWN;
}

DXGI_FORMAT DepthStencil::MapShaderResourceFormat(Usage usage)
{
	switch (usage)
	{
	case DepthStencil::Usage::duDEPTH_STENCIL:
		return DXGI_FORMAT::DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	case DepthStencil::Usage::duSHADOW_DEPTH:
		return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
	}

	return DXGI_FORMAT_UNKNOWN;
}