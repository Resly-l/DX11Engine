#include "counter_node.h"
#include "node_graph.h"

void CounterNode::InitializeSlots()
{
	pNodeGraph->AddSlot(this, { "delta time", Slot::Type::stOUTPUT, Slot::ValueType::svFLOAT });
	pNodeGraph->AddSlot(this, { "signal", Slot::Type::stOUTPUT, Slot::ValueType::svBOOL });
}

void CounterNode::Update(double deltaSeconds)
{
	data[outputs[0].valueKey] = deltaSeconds;

	bool counted = (elapsed += deltaSeconds) > pulse;

	if (counted)
	{
		elapsed = 0.0f;
	}

	data[outputs[1].valueKey] = counted;
}

JSON CounterNode::ToJson() const
{
	JSON json;

	json["pulse"] = pulse;

	return json;
}

void CounterNode::FromJson(const JSON& json)
{
	pulse = json["pulse"];
}

void CounterNode::DrawWidget()
{
	imnodes::BeginNode(GetUID(), "Counter", { 200, 200, 200, 255 });
	{
		ImGui::SetNextItemWidth(imnodes::defaultNodeWidth - ImGui::CalcTextSize("pulse cycle").x);
		ImGui::InputFloat("pulse cycle", &pulse);

		imnodes::OutputAttribute(outputs[0].GetUID(), outputs[0].valueKey.c_str());
		imnodes::OutputAttribute(outputs[1].GetUID(), outputs[1].valueKey.c_str());
	}
	imnodes::EndNode();
}