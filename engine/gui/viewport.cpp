#include "viewport.h"
#include "systems/render_system.h"
#include "rendering/render_target.h"

#include "input/keyboard.h"
#include "input/mouse.h"

void Viewport::Draw()
{
	auto pRenderSystem = SystemContainer::GetSystem<RenderSystem>();

	ImGui::Begin("Viewport", nullptr, windowFlags | ImGuiWindowFlags_NoScrollbar);
	{
		float fImageWidth = ImGui::GetContentRegionAvail().x;
		float fImageHeight = fImageWidth * 9.0f / 16.0f;

		ImGui::SetCursorPosY((ImGui::GetWindowHeight() + ImGui::GetStyle().DisplayWindowPadding.y - fImageHeight) * 0.5f);
		ImGui::Image(pRenderSystem->GetFramebufferRenderTarget().GetShaderResourceView(), { fImageWidth, fImageHeight });

		if (ImGui::IsWindowHovered() && ImGui::IsMouseDragging(ImGuiMouseButton_Right))
		{
			SystemContainer::GetSystem<RenderSystem>()->SetEditorCameraEngagement(true);
		}
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Right) == false)
		{
			SystemContainer::GetSystem<RenderSystem>()->SetEditorCameraEngagement(false);
		}
	}
	ImGui::End();
}