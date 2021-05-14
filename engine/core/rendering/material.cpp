#include "material.h"
#include "texture.h"
#include "constant_buffer.h"

Material::Material(const aiMaterial& ai_material, const std::string& filePath)
{
	std::filesystem::path path = filePath;
	std::string basePath = path.parent_path().string() + "/";

	aiString texturePath;

	// albedo
	{
		if (ai_material.GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == aiReturn_SUCCESS)
		{
			std::string filePath = basePath + texturePath.C_Str();
			pAlbedoMap = ResourceCodex::Resolve<Texture>("tex#" + filePath, Texture::Type::ttALBEDO, filePath);
			property.bUseAlbedoMap = TRUE;
		}
		else
		{
			aiColor4D diffuse = {};
			ai_material.Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
			property.albedo = { diffuse.r, diffuse.g, diffuse.b, diffuse.a };
		}
	}

	// normal
	{
		if (ai_material.GetTexture(aiTextureType_NORMALS, 0, &texturePath) == aiReturn_SUCCESS)
		{
			std::string filePath = basePath + texturePath.C_Str();
			pNormalMap = ResourceCodex::Resolve<Texture>("tex#" + filePath, Texture::Type::ttNORMAL, filePath);
			property.bUseNormalMap = TRUE;
		}
	}

	// specular
	{
		if (ai_material.GetTexture(aiTextureType_SPECULAR, 0, &texturePath) == aiReturn_SUCCESS)
		{
			std::string filePath = basePath + texturePath.C_Str();
			pSpecularMap = ResourceCodex::Resolve<Texture>("tex#" + filePath, Texture::Type::ttSPECULAR, filePath);
			property.bUseSpecularMap = TRUE;
		}
		else
		{
			aiColor3D specularColor = {};
			if (ai_material.Get(AI_MATKEY_COLOR_SPECULAR, specularColor) == aiReturn_SUCCESS)
			{
				property.specular = { specularColor.r, specularColor.g, specularColor.b, 0.0f };
			}

			float fShininess = 0.0f;
			if (ai_material.Get(AI_MATKEY_SHININESS, fShininess) == aiReturn_SUCCESS)
			{
				property.fShininess = fShininess;
			}
		}
	}

	pConstantBuffer = std::make_unique<ConstantBuffer>(ConstantType::ctMATERIAL, ShaderBindFlag::bfBIND_PS, property);
}

void Material::Bind() const
{
	if (pAlbedoMap) pAlbedoMap->Bind();
	if (pNormalMap) pNormalMap->Bind();
	if (pSpecularMap) pSpecularMap->Bind();

	pConstantBuffer->Bind();
}