#pragma once
#include "node.h"

class EventEmitterNode : public Node<EventEmitterNode>
{
private:
	std::string eventName;

public:
	void InitializeSlots() override;
	void Update(double deltaSeconds) override;

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;
};