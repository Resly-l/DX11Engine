#include "event_emitter_node.h"
#include "node_graph.h"
#include "event.h"

void EventEmitterNode::InitializeSlots()
{
	pNodeGraph->AddSlot(this, { "condition", Slot::Type::stINPUT, Slot::ValueType::svBOOL });
}

void EventEmitterNode::Update(double deltaSeconds)
{
	if (data[inputs[0].valueKey])
	{
		EventManager::Emit({ eventName });
	}
}

JSON EventEmitterNode::ToJson() const
{
	JSON json;

	json["eventName"] = eventName;

	return json;
}

void EventEmitterNode::FromJson(const JSON& json)
{
	eventName = json["eventName"];
}

void EventEmitterNode::DrawWidget()
{
	imnodes::BeginNode(GetUID(), "Event Emitter", { 200.0f, 100.0f, 100.0f, 255.0f });
	{
		ImGui::SetNextItemWidth(imnodes::defaultNodeWidth - ImGui::CalcTextSize("event name").x);
		ImGui::InputText("event name", &eventName);

		imnodes::InputAttribute(inputs[0].GetUID(), "condition");
	}
	imnodes::EndNode();
}