#pragma once
#include "node.h"

class VectorNode : public Node<VectorNode>
{
public:
	void InitializeSlots() override;
	void Update(double deltaSeconds) override;

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;

private:
	Vector vector;
};