#include "system_viewer.h"
#include "systems/render_system.h"

SystemViewer::SystemViewer()
{
	systemNames[RenderSystem::ID] = "Render System";
}

void SystemViewer::Draw() 
{
	ImGui::Begin("System Viewer");
	{
		for (auto& pSystem : SystemContainer::GetSystems())
		{
			if (ImGui::CollapsingHeader(systemNames[pSystem->GetID()].c_str()))
			{
				bool bActive = pSystem->IsActive();
				ImGui::Checkbox("active", &bActive);
				pSystem->SetActiveness(bActive);

				ImGui::NewLine();
				pSystem->DrawWidget();
			}
		}
	}
	ImGui::End();
}