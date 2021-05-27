#include "vector_decomposer_node.h"
#include "node_graph.h"

void VectorDecomposerNode::InitializeSlots()
{
	pNodeGraph->AddSlot(this, { "vector", Slot::Type::stINPUT, Slot::ValueType::svVECTOR });

	pNodeGraph->AddSlot(this, { "x", Slot::Type::stOUTPUT, Slot::ValueType::svFLOAT });
	pNodeGraph->AddSlot(this, { "y", Slot::Type::stOUTPUT, Slot::ValueType::svFLOAT });
	pNodeGraph->AddSlot(this, { "z", Slot::Type::stOUTPUT, Slot::ValueType::svFLOAT });
	pNodeGraph->AddSlot(this, { "w", Slot::Type::stOUTPUT, Slot::ValueType::svFLOAT });
}

void VectorDecomposerNode::Update(double deltaSeconds)
{
	data[outputs[0].valueKey] = data.value(inputs[0].valueKey, Vector{}).x;
	data[outputs[1].valueKey] = data.value(inputs[0].valueKey, Vector{}).y;
	data[outputs[2].valueKey] = data.value(inputs[0].valueKey, Vector{}).z;
	data[outputs[3].valueKey] = data.value(inputs[0].valueKey, Vector{}).w;
}

void VectorDecomposerNode::DrawWidget()
{
	imnodes::BeginNode(GetUID(), "Decompose Vector", { 64, 32, 178, 255 });
	{
		imnodes::InputAttribute(inputs[0].GetUID(), inputs[0].valueKey.c_str());

		for (auto& output : outputs)
		{
			imnodes::OutputAttribute(output.GetUID(), output.valueKey.c_str());
		}
	}
	imnodes::EndNode();
}