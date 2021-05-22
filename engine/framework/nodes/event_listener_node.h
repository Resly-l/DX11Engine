#pragma once
#include "node.h"

class EventListenerNode : public Node<EventListenerNode>
{
private:
	std::string prevEventName;
	std::string eventName;
	bool result = false;

public:
	void InitializeSlots() override;
	void Update(double deltaSeconds) override;

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;
};