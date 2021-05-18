#include "entity_inspector.h"
#include "scene_manager.h"

void EntityInspector::Draw()
{
	ImGui::Begin("Inspector");
	{
		if (auto pScene = SceneManager::GetActiveScene())
		{
			if (auto pEntity = pScene->GetSelectedEntity())
			{
				EditEntityName(pEntity);
				ShowEntityInfo(pEntity);
				EditEntityPedigree(pScene, pEntity);

				if (pEntity == nullptr)
				{
					ImGui::End();
					return;
				}

				ImGui::NewLine();
				ImGui::Separator();

				EditEntityComponents(pEntity);
				ImGui::NewLine();
				DrawEntityComponentWidgets(pEntity);
			}
		}
	}
	ImGui::End();
}

void EntityInspector::EditEntityName(Entity* pEntity)
{
	ImGui::NewLine();
	auto entityName = pEntity->GetName();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("selected entity :");
	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
	ImGui::InputText(" ", &entityName, 256);
	pEntity->SetName(entityName);
}

void EntityInspector::ShowEntityInfo(Entity* pEntity)
{
	ImGui::NewLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text(("parent : " + (pEntity->GetParent() ? pEntity->GetParent()->GetName() : "none")).c_str());
	ImGui::AlignTextToFramePadding();
	ImGui::Text(("child count : " + std::to_string(pEntity->GetChildren().size())).c_str());
	ImGui::NewLine();
}

void EntityInspector::EditEntityPedigree(Scene* pScene, Entity*& pEntity)
{
	if (ImGui::Button("Add Child", { ImGui::GetContentRegionAvail().x * 0.5f - (ImGui::GetStyle().ItemSpacing.x), 0.0f }))
	{
		pEntity->AddChild(std::make_unique<Entity>());
	}

	ImGui::SameLine();

	if (ImGui::Button("Delete", { ImGui::GetContentRegionAvail().x, 0.0f }))
	{
		if (auto pParent = pEntity->GetParent())
		{
			pParent->RemoveChild(pEntity);
		}
		pScene->SelectEntity(nullptr);
		pEntity = nullptr;
	}
}

void EntityInspector::EditEntityComponents(Entity* pEntity)
{
	ImGui::SetCursorPos({ (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("[Component Configuration]").x) * 0.5f, ImGui::GetCursorPosY() });
	ImGui::AlignTextToFramePadding();
	ImGui::Text("[Component Configuration]");

	ImGui::NewLine();
	if (ImGui::BeginCombo("components", ComponentFactory::GetRegisteredStringIDs()[uComponentIndex].c_str()))
	{
		for (uint32_t uID = 0; uID < ComponentFactory::GetRegisteredStringIDs().size(); uID++)
		{
			if (ImGui::Selectable(ComponentFactory::GetRegisteredStringIDs()[uID].c_str()))
			{
				uComponentIndex = uID;
			}
		}

		ImGui::EndCombo();
	}

	ImGui::NewLine();
	if (ImGui::Button("Assign Component"))
	{
		pEntity->AssignComponent(ComponentFactory::GetRegisteredStringIDs()[uComponentIndex]);
	}
}

void EntityInspector::DrawEntityComponentWidgets(Entity* pEntity)
{
	if (ImGui::BeginTabBar("components", ImGuiTabBarFlags_FittingPolicyScroll))
	{
		for (const auto& stringID : ComponentFactory::GetRegisteredStringIDs())
		{
			if (auto pComponent = pEntity->GetComponent(stringID))
			{
				if (ImGui::BeginTabItem(stringID.c_str()))
				{
					ImGui::NewLine();
					pComponent->DrawWidget();

					ImGui::NewLine();
					if (ImGui::Button("Remove Component"))
					{
						pEntity->RemoveComponent(stringID);
					}

					ImGui::EndTabItem();
				}
			}
		}

		ImGui::EndTabBar();
	}
}