#include "boolean_node.h"
#include "node_graph.h"

void BooleanNode::InitializeSlots()
{
	pNodeGraph->AddSlot(this, { "value", Slot::Type::stOUTPUT, Slot::ValueType::svBOOL });
}

void BooleanNode::Update(double deltaSeconds)
{
	data[outputs[0].valueKey] = value;
}

JSON BooleanNode::ToJson() const
{
	JSON json;

	json["value"] = value;

	return json;
}

void BooleanNode::FromJson(const JSON& json)
{
	value = json["value"];
}

void BooleanNode::DrawWidget()
{
	imnodes::BeginNode(GetUID(), "Boolean", { 128, 32, 64, 255 });
	{
		ImGui::SetNextItemWidth(imnodes::defaultNodeWidth - ImGui::CalcTextSize("value").x);
		ImGui::Checkbox("value", &value);

		imnodes::OutputAttribute(outputs[0].GetUID(), outputs[0].valueKey.c_str());
	}
	imnodes::EndNode();
}