#pragma once
#include "node.h"

class CounterNode : public Node<CounterNode>
{
private:
	float elapsed = 0.0f;
	float pulse = 1.0f;

public:
	void InitializeSlots() override;
	void Update(double deltaSeconds) override;

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;
};