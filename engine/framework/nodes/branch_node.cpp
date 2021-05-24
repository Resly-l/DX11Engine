#include "branch_node.h"
#include "node_graph.h"

void BranchNode::InitializeSlots()
{
	pNodeGraph->AddSlot(this, { "condition", Slot::Type::stINPUT, Slot::ValueType::svBOOL });

	pNodeGraph->AddSlot(this, { "true", Slot::Type::stOUTPUT, Slot::ValueType::svBOOL });
	pNodeGraph->AddSlot(this, { "false", Slot::Type::stOUTPUT, Slot::ValueType::svBOOL });
}

void BranchNode::Update(double deltaSeconds)
{
	bool condition = data[inputs[0].valueKey];

	data[outputs[0].valueKey] = condition;
	data[outputs[1].valueKey] = !condition;
}

void BranchNode::DrawWidget()
{
	imnodes::BeginNode(GetUID(), "Branch", { 100, 100, 100, 255 });
	{
		imnodes::InputAttribute(inputs[0].GetUID(), inputs[0].valueKey.c_str());

		imnodes::OutputAttribute(outputs[0].GetUID(), outputs[0].valueKey.c_str());
		imnodes::OutputAttribute(outputs[1].GetUID(), outputs[1].valueKey.c_str());
	}
	imnodes::EndNode();
}