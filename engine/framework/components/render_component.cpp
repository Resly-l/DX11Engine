#include "render_component.h"
#include "rendering/mesh.h"
#include "rendering/material.h"
#include "rendering/render_queue.h"
#include "console.h"
#include "rendering/constant_buffer.h"

#include "entity.h"
#include "components/transform_component.h"

std::unique_ptr<ConstantBuffer> RenderComponent::pTransformCB;

RenderComponent::RenderComponent()
{
	if (pTransformCB == nullptr)
	{
		pTransformCB = std::make_unique<ConstantBuffer>(ConstantType::ctINSTANCE_TRANSFORM, ShaderBindFlag::bfBIND_VS, Matrix{});
	}
}

bool RenderComponent::LoadModel(const std::string& filePath)
{
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

	for (uint32_t uMaterial = 0; uMaterial < pScene->mNumMaterials; uMaterial++)
	{
		auto& ai_material = *pScene->mMaterials[uMaterial];

		materialPtrs.push_back(ResourceCodex::Resolve<Material>(filePath + ai_material.GetName().C_Str(), ai_material, filePath));
	}

	for (uint32_t uMesh = 0; uMesh < pScene->mNumMeshes; uMesh++)
	{
		auto& ai_mesh = *pScene->mMeshes[uMesh];

		meshPtrs.push_back(ResourceCodex::Resolve<Mesh>(filePath + "#mesh#" + ai_mesh.mName.C_Str(), ai_mesh));
		meshPtrs.back()->SetMaterial(materialPtrs[ai_mesh.mMaterialIndex]);

		boundingBox += meshPtrs.back()->GetBoundingBox();
	}

	return true;
}

void RenderComponent::Submit(RenderQueue& renderQueue)
{
	renderQueue.QueueDrawable(bAlpha ? RenderPass::Tag::ptALPHA : RenderPass::Tag::ptGBUFFER, this);

	if (bCastShadow)
	{
		renderQueue.QueueDrawable(RenderPass::Tag::ptSHADOW, this);
	}
}

void RenderComponent::Draw()
{
	if (meshPtrs.empty())
	{
		return;
	}

	Matrix mTransform;
	
	if (auto pTransformComponent = pOwner->GetComponent<TransformComponent>())
	{
		mTransform = pTransformComponent->GetTransformMatrix();
	}

	pTransformCB->Update(mTransform);
	pTransformCB->Bind();

	for (const auto& pMesh : meshPtrs)
	{
		pMesh->Draw();
	}
}

JSON RenderComponent::ToJson() const
{
	JSON json;

	json["file_path"] = filePath;
	json["bCastShadow"] = bCastShadow;
	json["bAlpha"] = bAlpha;

	return json;
}

void RenderComponent::FromJson(const JSON& json)
{
	filePath = json["filePath"];
	bCastShadow = json["bCastShadow"];
	bAlpha = json["bAlpha"];

	if (filePath.empty() == false)
	{
		LoadModel(filePath);
	}
}

void RenderComponent::DrawWidget()
{
	if (ImGui::Button("Load Model"))
	{
		OPENFILENAMEA ofn;
		char szFile[260] = { 0 };

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = GetActiveWindow();
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = "Model(*.fbx; *.obj)\0*.obj;*.fbx;\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = nullptr;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = "..\\asset\\models";
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			LoadModel(std::filesystem::relative(ofn.lpstrFile).string());
		}
	}


	ImGui::AlignTextToFramePadding();
	ImGui::Text(("file path : " + filePath).c_str());

	ImGui::Checkbox("cast shadow", &bCastShadow);
	ImGui::Checkbox("alpha", &bAlpha);
}