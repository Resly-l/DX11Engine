#include "texture.h"
#include "renderer.h"
#include "console.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

void Internal::TextureBase::Bind() const
{
	Renderer::GetContext()->PSSetShaderResources(slot, 1, pShaderResourceView.GetAddressOf());
}

Texture::Texture(Type type, const std::string& filePath)
{
	this->filePath = filePath;
	this->slot = (uint32_t)type;

	int width = 0, height = 0;
	int bitPerChannel = 0;

	byte* pData = stbi_load(filePath.c_str(), &width, &height, &bitPerChannel, 4);

	if (pData == nullptr)
	{
		Console::AddLog({ Log::Type::ltERROR, "failed to load texture : " + filePath });
		return;
	}

	ComPtr<ID3D11Texture2D> pTexture2D;
	{
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MipLevels = 0;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;

		if (FAILED(Renderer::GetDevice()->CreateTexture2D(&textureDesc, nullptr, &pTexture2D)))
		{
			Console::AddLog({ Log::Type::ltERROR, "failed to create texture2d : " + filePath });
			return;
		}
		Renderer::GetContext()->UpdateSubresource(pTexture2D.Get(), 0, nullptr, pData, width * sizeof(UINT), 0);
	}
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1;

		Renderer::GetDevice()->CreateShaderResourceView(pTexture2D.Get(), &srvDesc, &pShaderResourceView);
		Renderer::GetContext()->GenerateMips(pShaderResourceView.Get());
	}
}

TextureCube::TextureCube(const std::string& dirPath)
{
	this->filePath = dirPath;
	this->slot = uint32_t(Texture::Type::ttSKY);

	int width = 0, height = 0;
	int iPrevWidth = 0, iPrevHeight = 0; // width & height of previously loaded image
	int iBitPerChannel = 0;
	bool bSizeMismatch = false;

	byte* imagePtrs[6] = {};

	imagePtrs[1] = stbi_load((dirPath + "nx.png").c_str(), &width, &height, &iBitPerChannel, 4);
	iPrevWidth = width, iPrevHeight = height;

	imagePtrs[3] = stbi_load((dirPath + "ny.png").c_str(), &width, &height, &iBitPerChannel, 4);
	bSizeMismatch |= (width != iPrevWidth) || (height != iPrevHeight);
	iPrevWidth = width, iPrevHeight = height;

	imagePtrs[5] = stbi_load((dirPath + "nz.png").c_str(), &width, &height, &iBitPerChannel, 4);
	bSizeMismatch |= (width != iPrevWidth) || (height != iPrevHeight);
	iPrevWidth = width, iPrevHeight = height;

	imagePtrs[0] = stbi_load((dirPath + "px.png").c_str(), &width, &height, &iBitPerChannel, 4);
	bSizeMismatch |= (width != iPrevWidth) || (height != iPrevHeight);
	iPrevWidth = width, iPrevHeight = height;

	imagePtrs[2] = stbi_load((dirPath + "py.png").c_str(), &width, &height, &iBitPerChannel, 4);
	bSizeMismatch |= (width != iPrevWidth) || (height != iPrevHeight);
	iPrevWidth = width, iPrevHeight = height;

	imagePtrs[4] = stbi_load((dirPath + "pz.png").c_str(), &width, &height, &iBitPerChannel, 4);
	bSizeMismatch |= (width != iPrevWidth) || (height != iPrevHeight);

	if (bSizeMismatch)
	{
		Console::AddLog({ Log::Type::ltERROR, "failed to load texture-cube image, texture size mismatch" });
		return;
	}

	ComPtr<ID3D11Texture2D> pTexture2D;
	{
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.ArraySize = 6;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MipLevels = 1;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;

		D3D11_SUBRESOURCE_DATA sd[6] = {};
		for (int i = 0; i < 6; i++)
		{
			sd[i].pSysMem = imagePtrs[i];
			sd[i].SysMemPitch = width * 4;
		}

		if (FAILED(Renderer::GetDevice()->CreateTexture2D(&textureDesc, sd, &pTexture2D)))
		{
			Console::AddLog({ Log::Type::ltERROR, "failed to create texture-cube textures: " + dirPath });
			return;
		}
	}
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;

		Renderer::GetDevice()->CreateShaderResourceView(pTexture2D.Get(), &srvDesc, &pShaderResourceView);
	}
}