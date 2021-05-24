#include "event_listener_node.h"
#include "node_graph.h"
#include "event.h"

void EventListenerNode::InitializeSlots()
{
	pNodeGraph->AddSlot(this, { "result", Slot::Type::stOUTPUT, Slot::ValueType::svBOOL });
}

void EventListenerNode::Update(double deltaSeconds)
{
	if (prevEventName != eventName)
	{
		prevEventName = eventName;

		// unsubscribe previously subscribed event name
		EventManager::Unsubscribe(subscriptionKey);

		EventManager::Subscribe(eventName,
			[this](const Event& event)
			{
				result = true;
			},
			subscriptionKey);
	}

	data[outputs[0].valueKey] = result;

	result = false;
}

JSON EventListenerNode::ToJson() const
{
	JSON json;

	json["eventName"] = eventName;
	json["subscriptionKey"] = subscriptionKey;

	return json;
}

void EventListenerNode::FromJson(const JSON& json)
{
	eventName = json["eventName"];
	subscriptionKey = json["subscriptionKey"];
}

void EventListenerNode::DrawWidget()
{
	imnodes::BeginNode(GetUID(), "Event Listener", { 100.0f, 200.0f, 255.0f, 255.0f });
	{
		ImGui::SetNextItemWidth(imnodes::defaultNodeWidth - ImGui::CalcTextSize("event name").x);
		ImGui::InputText("event name", &eventName);

		ImGui::SetNextItemWidth(imnodes::defaultNodeWidth - ImGui::CalcTextSize("subscription key").x);
		ImGui::InputText("subscription key", &subscriptionKey);

		imnodes::OutputAttribute(outputs[0].GetUID(), "result");
	}
	imnodes::EndNode();
}