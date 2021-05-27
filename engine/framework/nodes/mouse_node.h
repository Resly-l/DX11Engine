#pragma once
#include "node.h"

class MouseNode : public Node<MouseNode>
{
private:
	int button = 1; // virtual key code

public:
	void InitializeSlots() override;
	void Update(double deltaSeconds) override;

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;
};