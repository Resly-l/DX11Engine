#include "workspace.h"
#include "scene_manager.h"
#include "system_container.h"

#include "systems/render_system.h"
#include "rendering/render_target.h"
#include "components/script_component.h"

#include "input/keyboard.h"
#include "input/mouse.h"

void Workspace::Draw()
{
	auto pRenderSystem = SystemContainer::GetSystem<RenderSystem>();

	ImGui::Begin("Workspace", nullptr, windowFlags | ImGuiWindowFlags_NoScrollbar);
	ImGui::BeginTabBar("workspace");
	{
		if (ImGui::BeginTabItem("Viewport"))
		{
			float fImageWidth = ImGui::GetContentRegionAvail().x;
			float fImageHeight = fImageWidth * 9.0f / 16.0f;

			ImGui::Image(pRenderSystem->GetFramebufferRenderTarget().GetShaderResourceView(), { fImageWidth, fImageHeight });

			if (ImGui::IsWindowHovered() && ImGui::IsMouseDragging(ImGuiMouseButton_Right))
			{
				SystemContainer::GetSystem<RenderSystem>()->SetEditorCameraEngagement(true);
			}
			if (ImGui::IsMouseDragging(ImGuiMouseButton_Right) == false)
			{
				SystemContainer::GetSystem<RenderSystem>()->SetEditorCameraEngagement(false);
			}

			ImGui::EndTabItem();
		}
		
		if (ImGui::BeginTabItem("Script"))
		{
			if (auto pScene = SceneManager::GetActiveScene())
			{
				if (auto pEntity = pScene->GetSelectedEntity())
				{
					if (auto pScriptComponent = pEntity->GetComponent<ScriptComponent>())
					{
						pScriptComponent->DrawEditor();
					}
				}
			}

			ImGui::EndTabItem();
		}
	}
	ImGui::EndTabBar();
	ImGui::End();
}