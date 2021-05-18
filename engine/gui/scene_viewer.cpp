#include "scene_viewer.h"
#include "scene_manager.h"
#include "entity.h"

#include "components/transform_component.h"

SceneViewer::SceneViewer()
{
	treeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick;
}

void SceneViewer::Draw()
{
	ImGui::Begin("Scene Viewer");
	{
		if (auto pScene = SceneManager::GetActiveScene())
		{

			const auto& style = ImGui::GetStyle();
			const auto availableSpace = ImGui::GetContentRegionAvail();
			ImGui::BeginChildFrame(1, { availableSpace.x, availableSpace.y - (style.IndentSpacing + style.ItemSpacing.y) * 2.0f });
			{

				ImGui::AlignTextToFramePadding();
				if (ImGui::TreeNodeEx(pScene, treeFlags, pScene->GetName().c_str()))
				{
					bool bSelected = false;

					DrawEntityHierarchy(pScene, pScene->GetRootEntity(), bSelected);
					ImGui::TreePop();
				}
			}
			ImGui::EndChildFrame();

			EditSceneName(pScene);
			EditCameraName(pScene);
		}
	}
	ImGui::End();
}

void SceneViewer::DrawEntityHierarchy(Scene* pScene, Entity* pEntity, bool& bSelected)
{
	if (ImGui::TreeNodeEx(pEntity, treeFlags | (pScene->GetSelectedEntity() == pEntity ? ImGuiTreeNodeFlags_Selected : 0), pEntity->GetName().c_str()))
	{
		for (auto& pChild : pEntity->GetChildren())
		{
			DrawEntityHierarchy(pScene, pChild.get(), bSelected);

			if (ImGui::IsItemClicked() && bSelected == false)
			{
				bSelected = true;
				pScene->SelectEntity(pChild.get());
			}
		}

		ImGui::TreePop();
	}

	if (ImGui::IsItemClicked() && bSelected == false)
	{
		bSelected = true;
		pScene->SelectEntity(pEntity);
	}
}

void SceneViewer::EditSceneName(Scene* pScene)
{
	std::string sceneName = pScene ? pScene->GetName() : "";

	ImGui::AlignTextToFramePadding();
	ImGui::InputText("scene name", &sceneName, 256);

	if (pScene) pScene->SetName(sceneName);
}

void SceneViewer::EditCameraName(Scene* pScene)
{
	std::string cameraName = pScene ? pScene->GetCameraName() : "";

	ImGui::AlignTextToFramePadding();
	ImGui::InputText("camera name", &cameraName, 256);

	if (pScene) pScene->SetCameraName(cameraName);
}