#include "mouse_node.h"
#include "node_graph.h"
#include "input/mouse.h"

void MouseNode::InitializeSlots()
{
	pNodeGraph->AddSlot(this, { "is down", Slot::Type::stOUTPUT, Slot::ValueType::svBOOL });
	pNodeGraph->AddSlot(this, { "is hold", Slot::Type::stOUTPUT, Slot::ValueType::svBOOL });
	pNodeGraph->AddSlot(this, { "is free", Slot::Type::stOUTPUT, Slot::ValueType::svBOOL });

	pNodeGraph->AddSlot(this, { "pos x", Slot::Type::stOUTPUT, Slot::ValueType::svFLOAT });
	pNodeGraph->AddSlot(this, { "pos y", Slot::Type::stOUTPUT, Slot::ValueType::svFLOAT });
	pNodeGraph->AddSlot(this, { "delta x", Slot::Type::stOUTPUT, Slot::ValueType::svFLOAT });
	pNodeGraph->AddSlot(this, { "delta y", Slot::Type::stOUTPUT, Slot::ValueType::svFLOAT });
}

void MouseNode::Update(double deltaSeconds)
{
	data[outputs[0].valueKey] = Mouse::IsButtonDown(button);
	data[outputs[1].valueKey] = Mouse::IsButtonHold(button);
	data[outputs[2].valueKey] = Mouse::IsButtonFree(button);

	POINT totalDelta = {};
	for (auto& delta : Mouse::GetRawDeltas())
	{
		totalDelta.x += delta.x;
		totalDelta.y += delta.y;
	}

	data[outputs[3].valueKey] = Mouse::GetPosition().x;
	data[outputs[4].valueKey] = Mouse::GetPosition().y;
	data[outputs[5].valueKey] = totalDelta.x;
	data[outputs[6].valueKey] = totalDelta.y;
}

JSON MouseNode::ToJson() const
{
	JSON json;

	json["button"] = button;

	return json;
}

void MouseNode::FromJson(const JSON& json)
{
	button = json["button"];
}

void MouseNode::DrawWidget()
{
	imnodes::BeginNode(GetUID(), "Mouse", { 239, 179, 247, 255 });
	{
		ImGui::Text("button");
		ImGui::RadioButton("L", &button, 1);
		ImGui::SameLine();
		ImGui::RadioButton("R", &button, 2);
		ImGui::SameLine();
		ImGui::RadioButton("M", &button, 3);
		ImGui::NewLine();

		for (auto& output : outputs)
		{
			imnodes::OutputAttribute(output.GetUID(), output.valueKey.c_str());
		}
	}
	imnodes::EndNode();
}