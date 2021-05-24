#pragma once
#include "node.h"

class MouseNode : public Node<MouseNode>
{
public:
	void InitializeSlots() override;
	void Update(double deltaSeconds) override;

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;

private:
	int button = 1;
};