#include "render_target.h"
#include "renderer.h"
#include"depth_stencil.h"
#include "console.h"

RenderTarget::RenderTarget(Usage usage)
	:
	usage(usage),
	width(Renderer::GetSwapChainWidth()),
	height(Renderer::GetSwapChainHeight()),
	syncResolution(true)
{
	Initialize();
}

RenderTarget::RenderTarget(Usage usage, uint32_t width, uint32_t height)
	:
	usage(usage),
	width(width),
	height(height),
	syncResolution(false)
{
	Initialize();
}

void RenderTarget::BindAsTarget(DepthStencil* pDepthStencil)
{
	if (syncResolution)
	{
		const uint32_t swapChainWidth = Renderer::GetSwapChainWidth();
		const uint32_t swapChainHeight = Renderer::GetSwapChainHeight();

		if (width != swapChainWidth || height != swapChainHeight)
		{
			width = swapChainWidth;
			height = swapChainHeight;

			Initialize();
		}
	}

	Renderer::GetContext()->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencil ? pDepthStencil->GetView() : nullptr);
}

void RenderTarget::BindAsTexture(uint32_t slot)
{
	Renderer::GetContext()->PSSetShaderResources(slot, 1, pShaderResourceView.GetAddressOf());
}

void RenderTarget::Clear(const Vector& clearColor)
{
	Renderer::GetContext()->ClearRenderTargetView(pRenderTargetView.Get(), &clearColor[0]);
}

ID3D11RenderTargetView* RenderTarget::GetView()
{
	if (syncResolution)
	{
		const uint32_t swapChainWidth = Renderer::GetSwapChainWidth();
		const uint32_t swapChainHeight = Renderer::GetSwapChainHeight();

		if (width != swapChainWidth || height != swapChainHeight)
		{
			width = swapChainWidth;
			height = swapChainHeight;

			Initialize();
		}
	}

	return pRenderTargetView.Get();
}

void RenderTarget::Initialize()
{
	ComPtr<ID3D11Texture2D> pTexture2D;
	{
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.ArraySize = 1;
		textureDesc.Format = MapFormat(usage);
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.MiscFlags = 0;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MipLevels = 1;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;

		if (FAILED(Renderer::GetDevice()->CreateTexture2D(&textureDesc, nullptr, &pTexture2D)))
		{
			Console::AddLog({ Log::Type::ltERROR, "failed to create render target texture" });
		}
	}
	{
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = MapFormat(usage);
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };

		if (FAILED(Renderer::GetDevice()->CreateRenderTargetView(pTexture2D.Get(), &rtvDesc, &pRenderTargetView)))
		{
			Console::AddLog({ Log::Type::ltERROR, "failed to create render target view" });
		}
	}
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = MapFormat(usage);
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;

		if (FAILED(Renderer::GetDevice()->CreateShaderResourceView(pTexture2D.Get(), &srvDesc, &pShaderResourceView)))
		{
			Console::AddLog({ Log::Type::ltERROR, "failed to create render target srv" });
		}
	}
}

DXGI_FORMAT RenderTarget::MapFormat(Usage usage)
{
	switch (usage)
	{
	case Usage::ruRENDER_TARGET:
		return DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	case Usage::ruGBUFFER:
		return DXGI_FORMAT_R32G32B32A32_FLOAT;
		break;
	}

	return DXGI_FORMAT_UNKNOWN;
}