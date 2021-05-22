#include "event_emitter_node.h"
#include "node_graph.h"
#include "event.h"

void EventEmitterNode::InitializeSlots()
{
	pNodeGraph->AddSlot(this, { "condition", Slot::Type::stINPUT, Slot::ValueType::svBOOL });
}

void EventEmitterNode::Update(double deltaSeconds)
{
	if (data["condition"])
	{
		EventManager::Emit({ eventName });
	}
}

JSON EventEmitterNode::ToJson() const
{
	JSON json;

	json["event_name"] = eventName;

	return json;
}

void EventEmitterNode::FromJson(const JSON& json)
{
	eventName = json["event_name"];
}

void EventEmitterNode::DrawWidget()
{
	imnodes::BeginNode(GetUID(), "Event Listener", { 200.0f, 100.0f, 255.0f, 255.0f });
	{
		ImGui::SetNextItemWidth(imnodes::fDefaultNodeWidth - ImGui::CalcTextSize("event name").x);
		ImGui::InputText("event name", &eventName);

		imnodes::InputAttribute(inputs[0].GetUID(), "condition");
	}
	imnodes::EndNode();
}