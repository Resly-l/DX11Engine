#pragma once
#include "node.h"

class KeyboardNode : public Node<KeyboardNode>
{
private:
	unsigned char key;

public:
	void InitializeSlots() override;
	void Update(double deltaSeconds) override;

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;
};