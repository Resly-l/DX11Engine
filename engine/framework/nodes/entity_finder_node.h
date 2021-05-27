#pragma once
#include "node.h"

class EntityFinderNode : public Node<EntityFinderNode>
{
private:
	std::string entityName;

public:
	void InitializeSlots() override;
	void Update(double deltaSeconds) override;

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;
};