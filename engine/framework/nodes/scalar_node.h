#pragma once
#include "node.h"

class ScalarNode : public Node<ScalarNode>
{
public:
	void InitializeSlots() override;
	void Update(double deltaSeconds) override;

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;

private:
	float scalar;
};