#include "toolbar.h"
#include "console.h"

void Toolbar::Draw()
{
	ImGui::Begin("Toolbar");
	{
		if (ImGui::BeginTabBar("toolbar"))
		{
			if (ImGui::BeginTabItem("Console"))
			{
				Console::DrawWidget();

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
	ImGui::End();
}