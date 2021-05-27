#pragma once
#include "node.h"

class BooleanNode : public Node<BooleanNode>
{
private:
	bool value = true;

public:
	void InitializeSlots() override;
	void Update(double deltaSeconds) override;

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;
};