#include "keyboard_node.h"
#include "node_graph.h"
#include "input/keyboard.h"

void KeyboardNode::InitializeSlots()
{
	pNodeGraph->AddSlot(this, { "is down", Slot::Type::stOUTPUT, Slot::ValueType::svBOOL });
	pNodeGraph->AddSlot(this, { "is hold", Slot::Type::stOUTPUT, Slot::ValueType::svBOOL });
	pNodeGraph->AddSlot(this, { "is free", Slot::Type::stOUTPUT, Slot::ValueType::svBOOL });
}

void KeyboardNode::Update(double deltaSeconds)
{
	data[outputs[0].valueKey] = Keyboard::IsKeyDown(key);
	data[outputs[1].valueKey] = Keyboard::IsKeyHold(key);
	data[outputs[2].valueKey] = Keyboard::IsKeyFree(key);
}

JSON KeyboardNode::ToJson() const
{
	JSON json;

	json["key"] = key;

	return json;
}

void KeyboardNode::FromJson(const JSON& json)
{
	key = json["key"];
}

void KeyboardNode::DrawWidget()
{
	imnodes::BeginNode(GetUID(), "Keyboard", { 128, 138, 105, 255 });
	{
		ImGui::SetNextItemWidth(imnodes::defaultNodeWidth - ImGui::CalcTextSize("key").x);
		ImGui::InputText("key", reinterpret_cast<char*>(&key), 2);

		for (auto& output : outputs)
		{
			imnodes::OutputAttribute(output.GetUID(), output.valueKey.c_str());
		}
	}
	imnodes::EndNode();
}