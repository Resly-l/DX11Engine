#pragma once
#include "node.h"

class StringNode : public Node<StringNode>
{
private:
	std::string string;

public:
	void InitializeSlots() override;
	void Update(double deltaSeconds) override;

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;
};