#pragma once
#include "node.h"

class EventEmitterNode : public Node<EventEmitterNode>
{
public:
	void InitializeSlots() override;
	void Update(double deltaSeconds) override;

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;

private:
	std::string eventName;
};