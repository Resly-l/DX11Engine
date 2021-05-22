#include "model_component.h"
#include "console.h"
#include "file_manager.h"

#include "rendering/mesh.h"
#include "rendering/material.h"
#include "rendering/constant_buffer.h"

#include "entity.h"
#include "components/transform_component.h"

std::unique_ptr<ConstantBuffer> ModelComponent::pTransformCB;

ModelComponent::ModelComponent()
{
	if (pTransformCB == nullptr)
	{
		pTransformCB = std::make_unique<ConstantBuffer>(ConstantType::ctINSTANCE_TRANSFORM, ShaderBindFlag::bfBIND_VS, Matrix{});
	}
}

bool ModelComponent::LoadModel(const std::string& filePath)
{
	materialPtrs.clear();
	meshPtrs.clear();

	boundingBox = {};

	static Assimp::Importer importer;

	auto pScene = importer.ReadFile(filePath,
		aiProcess_ConvertToLeftHanded |
		aiProcess_JoinIdenticalVertices |
		aiProcess_Triangulate |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_LimitBoneWeights |
		aiProcess_ValidateDataStructure |
		aiProcess_OptimizeMeshes |
		aiProcess_SplitLargeMeshes |
		aiProcess_ImproveCacheLocality
	);

	if (pScene == nullptr)
	{
		Console::AddLog({ Log::Type::ltERROR, "failed to load model : " + filePath });
		return false;
	}

	this->filePath = filePath;

	for (uint32_t i = 0; i < pScene->mNumMaterials; i++)
	{
		auto& ai_material = *pScene->mMaterials[i];

		materialPtrs.push_back(ResourceCodex::Resolve<Material>(filePath + ai_material.GetName().C_Str(), ai_material, filePath));
	}

	for (uint32_t i = 0; i < pScene->mNumMeshes; i++)
	{
		auto& ai_mesh = *pScene->mMeshes[i];

		meshPtrs.push_back(ResourceCodex::Resolve<Mesh>(filePath + "#mesh#" + ai_mesh.mName.C_Str(), ai_mesh));
		meshPtrs.back()->SetMaterial(materialPtrs[ai_mesh.mMaterialIndex]);

		boundingBox += meshPtrs.back()->GetBoundingBox();
	}

	return true;
}

void ModelComponent::Draw()
{
	if (meshPtrs.empty())
	{
		return;
	}

	Matrix transform;
	
	if (auto pTransformComponent = GetOwner()->GetComponent<TransformComponent>())
	{
		transform = pTransformComponent->GetTransformMatrix();
	}

	pTransformCB->Update(transform);
	pTransformCB->Bind();

	for (const auto& pMesh : meshPtrs)
	{
		pMesh->Draw();
	}
}

JSON ModelComponent::ToJson() const
{
	JSON json;

	json["file_path"] = filePath;
	json["cast_shadow"] = castShadow;
	json["transparent"] = transparent;

	return json;
}

void ModelComponent::FromJson(const JSON& json)
{
	filePath = json["file_path"];
	castShadow = json["cast_shadow"];
	transparent = json["transparent"];

	if (filePath.empty() == false)
	{
		LoadModel(filePath);
	}
}

void ModelComponent::DrawWidget()
{
	if (ImGui::Button("Load Model From File"))
	{
		auto modelPath = FileManager::SelectFileToLoad("..\\asset\\models", "Model(*.fbx; *.obj)\0*.obj;*.fbx;\0");
		if (modelPath.empty() == false)
		{
			LoadModel(modelPath);
		}
	}

	ImGui::NewLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text(("file path : " + filePath).c_str());

	ImGui::NewLine();
	ImGui::Checkbox("visible", &visible);

	ImGui::NewLine();
	ImGui::Checkbox("cast shadow", &castShadow);
	ImGui::Checkbox("alpha", &transparent);
}