#pragma once
#include "resource.h"

namespace Internal
{
	class TextureBase : public Resource
	{
	protected:
		std::string filePath;

		ComPtr<ID3D11ShaderResourceView> pShaderResourceView;
		uint32_t uSlot;

	public:
		void Bind() const;
	};
}

class Texture : public Internal::TextureBase
{
public:
	// type is used as a shader slot when binding
	enum class Type
	{
		ttPOSITION,
		ttNORMAL,
		ttALBEDO,
		ttSPECULAR,
		ttSKY,
		ttSHADOW,
	};

public:
	Texture(Type type, const std::string& filePath);
};

class TextureCube : public Internal::TextureBase
{
public:
	TextureCube(const std::string& dirPath);
};