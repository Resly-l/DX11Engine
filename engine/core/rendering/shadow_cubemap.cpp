#include "shadow_cubemap.h"
#include "renderer.h"
#include "console.h"

ShadowCubemap::ShadowCubemap(uint32_t uResulution)
{
	ComPtr<ID3D11Texture2D> pTexture2D;
	{
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = uResulution;
		textureDesc.Height = uResulution;
		textureDesc.ArraySize = 6;
		textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MipLevels = 1;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;

		if (FAILED(Renderer::GetDevice()->CreateTexture2D(&textureDesc, nullptr, &pTexture2D)))
		{
			Console::AddLog({ Log::Type::ltERROR, "failed to create shadow cubemap texture2d" });
			return;
		}
	}
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;

		Renderer::GetDevice()->CreateShaderResourceView(pTexture2D.Get(), &srvDesc, &pShaderResourceView);
	}

	for (uint32_t uFace = 0; uFace < depthBufferPtrs.size(); uFace++)
	{
		depthBufferPtrs[uFace] = std::make_unique<DepthStencil>(pTexture2D, uFace);
	}
}