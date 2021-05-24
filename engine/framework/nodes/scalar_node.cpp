#include "scalar_node.h"
#include "node_graph.h"

void ScalarNode::InitializeSlots()
{
	pNodeGraph->AddSlot(this, { "scalar", Slot::Type::stOUTPUT, Slot::ValueType::svFLOAT });
}

void ScalarNode::Update(double deltaSeconds)
{
	data["scalar"] = scalar;
}

JSON ScalarNode::ToJson() const
{
	JSON json;

	json["scalar"] = scalar;

	return json;
}

void ScalarNode::FromJson(const JSON& json)
{
	scalar = json["scalar"];
}

void ScalarNode::DrawWidget()
{
	imnodes::BeginNode(GetUID(), "Scalar", { 150, 150, 150, 255 });
	{
		ImGui::SetNextItemWidth(imnodes::defaultNodeWidth - ImGui::CalcTextSize("value").x);
		ImGui::InputFloat("value", &scalar);

		imnodes::OutputAttribute(outputs[0].GetUID(), outputs[0].valueKey.c_str());
	}
	imnodes::EndNode();
}