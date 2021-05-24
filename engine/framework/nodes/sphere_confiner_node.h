#pragma once
#include "node.h"

class SphereConfinerNode : public Node<SphereConfinerNode>
{
public:
	void InitializeSlots() override;
	void Update(double deltaSeconds) override;

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;

private:
	float radius = 2.0f;
};