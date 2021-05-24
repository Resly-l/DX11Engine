#pragma once
#include "node.h"

class EntityMoverNode : public Node<EntityMoverNode>
{
public:
	void InitializeSlots() override;
	void Update(double deltaSeconds) override;

	JSON ToJson() const override { return {}; }
	void FromJson(const JSON& json) override {}

	void DrawWidget() override;
};