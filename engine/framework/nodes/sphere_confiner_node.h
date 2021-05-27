#pragma once
#include "node.h"

class SphereConfinerNode : public Node<SphereConfinerNode>
{
private:
	float radius = 2.0f;

public:
	void InitializeSlots() override;
	void Update(double deltaSeconds) override;

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;
};