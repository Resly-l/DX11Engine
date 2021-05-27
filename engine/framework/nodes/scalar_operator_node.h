#pragma once
#include "node.h"

class ScalarOperatorNode : public Node<ScalarOperatorNode>
{
private:
	static const std::vector<std::string> operationNames;
	uint32_t operationIndex = 0;

public:
	void InitializeSlots() override;
	void Update(double deltaSeconds) override;

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;
};