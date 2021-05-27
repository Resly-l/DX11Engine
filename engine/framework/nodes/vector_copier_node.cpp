#include "vector_copier_node.h"
#include "node_graph.h"

void VectorCopierNode::InitializeSlots()
{
	pNodeGraph->AddSlot(this, { "src", Slot::Type::stINPUT, Slot::ValueType::svVECTOR });

	pNodeGraph->AddSlot(this, { "cpy0", Slot::Type::stOUTPUT, Slot::ValueType::svVECTOR });
	pNodeGraph->AddSlot(this, { "cpy1", Slot::Type::stOUTPUT, Slot::ValueType::svVECTOR });
}

void VectorCopierNode::Update(double deltaSeconds)
{
	data[outputs[0].valueKey] = data.value(inputs[0].valueKey, Vector());
	data[outputs[1].valueKey] = data.value(inputs[0].valueKey, Vector());
}

void VectorCopierNode::DrawWidget()
{
	imnodes::BeginNode(GetUID(), "Copy Vector", { 43, 74, 89, 255 });
	{
		imnodes::InputAttribute(inputs[0].GetUID(), inputs[0].valueKey.c_str());

		imnodes::OutputAttribute(outputs[0].GetUID(), outputs[0].valueKey.c_str());
		imnodes::OutputAttribute(outputs[1].GetUID(), outputs[1].valueKey.c_str());
	}
	imnodes::EndNode();
}