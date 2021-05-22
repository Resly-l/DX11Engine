#include "system_viewer.h"
#include "system_container.h"

void SystemViewer::Draw() 
{
	ImGui::Begin("System Viewer");
	{
		for (auto& pSystem : SystemContainer::GetSystems())
		{
			if (ImGui::CollapsingHeader(pSystem->GetStringID().c_str()))
			{
				ImGui::NewLine();
				pSystem->DrawWidget();
				ImGui::NewLine();
			}
		}
	}
	ImGui::End();
}