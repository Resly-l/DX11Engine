#include "string_node.h"
#include "node_graph.h"

void StringNode::InitializeSlots()
{
	pNodeGraph->AddSlot(this, { "string", Slot::Type::stOUTPUT, Slot::ValueType::svSTRING });
}

void StringNode::Update(double deltaSeconds)
{
	data[outputs[0].valueKey] = string;
}

JSON StringNode::ToJson() const
{
	JSON json;

	json["string"] = string;

	return json;
}

void StringNode::FromJson(const JSON& json)
{
	string = json["string"];
}

void StringNode::DrawWidget()
{
	imnodes::BeginNode(GetUID(), "String", { 255, 255, 20, 255 });
	{
		ImGui::SetNextItemWidth(imnodes::defaultNodeWidth - ImGui::CalcTextSize("string").x);
		ImGui::InputText("string", &string);

		imnodes::OutputAttribute(outputs[0].GetUID(), outputs[0].valueKey.c_str());
	}
	imnodes::EndNode();
}