#include "entity_finder_node.h"
#include "node_graph.h"
#include "scene_manager.h"

void EntityFinderNode::InitializeSlots()
{
	pNodeGraph->AddSlot(this, { "entity", Slot::Type::stOUTPUT, Slot::ValueType::svADDRESS });
}

void EntityFinderNode::Update(double deltaSeconds)
{
	data[outputs[0].valueKey] = reinterpret_cast<void*>(SceneManager::GetActiveScene()->FindEntity(entityName));
}

JSON EntityFinderNode::ToJson() const
{
	JSON json;

	json["entityName"] = entityName;

	return json;
}

void EntityFinderNode::FromJson(const JSON& json)
{
	entityName = json["entityName"];
}

void EntityFinderNode::DrawWidget()
{
	imnodes::BeginNode(GetUID(), "Find Entity", { 30, 247, 196, 255 });
	{
		ImGui::SetNextItemWidth(imnodes::defaultNodeWidth - ImGui::CalcTextSize("entity name").x);
		ImGui::InputText("entity name", &entityName);

		imnodes::OutputAttribute(outputs[0].GetUID(), outputs[0].valueKey.c_str());
	}
	imnodes::EndNode();
}