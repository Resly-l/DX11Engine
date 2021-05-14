#include "entity_inspector.h"
#include "scene.h"

#include "components/transform_component.h"
#include "components/render_component.h"
#include "components/camera_component.h"
#include "components/light_component.h"

EntityInspector::EntityInspector()
{
	componentIDs.push_back(std::make_pair("TransformComponent", std::make_tuple(TransformComponent::ID, TransformComponent::stringID)));
	componentIDs.push_back(std::make_pair("RenderComponent", std::make_tuple(RenderComponent::ID, RenderComponent::stringID)));
	componentIDs.push_back(std::make_pair("CameraComponent", std::make_tuple(CameraComponent::ID, CameraComponent::stringID)));
	componentIDs.push_back(std::make_pair("LightComponent", std::make_tuple(LightComponent::ID, LightComponent::stringID)));
}

void EntityInspector::Draw()
{
	ImGui::Begin("Inspector");
	{
		if (auto pScene = SceneManager::GetActiveScene())
		{
			if (auto pEntity = pScene->GetSelectedEntity())
			{
				EditEntityName(pEntity);
				DrawEntityInfo(pEntity);
				EditEntityPedigree(pScene, pEntity);

				if (pEntity == nullptr)
				{
					ImGui::End();
					return;
				}

				ImGui::NewLine();
				ImGui::Separator();

				EditEntityComponents(pEntity);
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

void EntityInspector::DrawEntityInfo(Entity* pEntity)
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
	if (ImGui::BeginCombo("components", componentIDs[uComponentIndex].first.c_str()))
	{
		for (uint32_t uID = 0; uID < componentIDs.size(); uID++)
		{
			if (ImGui::Selectable(componentIDs[uID].first.c_str()))
			{
				uComponentIndex = uID;
			}
		}

		ImGui::EndCombo();
	}

	ImGui::NewLine();
	if (ImGui::Button("Assign Component"))
	{
		auto ID = componentIDs[uComponentIndex].second;

		pEntity->AssignComponent(std::get<ComponentID>(ID), std::get<std::string>(ID));
	}

	ImGui::NewLine();
	if (ImGui::BeginTabBar("components", ImGuiTabBarFlags_FittingPolicyScroll))
	{
		for (auto& [componentName, ID] : componentIDs)
		{
			if (auto pComponent = pEntity->GetComponent(std::get<ComponentID>(ID)))
			{
				if (ImGui::BeginTabItem(componentName.c_str()))
				{
					ImGui::NewLine();
					pComponent->DrawWidget();

					ImGui::NewLine();
					if (ImGui::Button("Remove Component"))
					{
						pEntity->RemoveComponent(std::get<ComponentID>(ID));
					}

					ImGui::EndTabItem();
				}
			}
		}

		ImGui::EndTabBar();
	}
}