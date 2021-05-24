#include "vector_node.h"
#include "node_graph.h"

void VectorNode::InitializeSlots()
{
	pNodeGraph->AddSlot(this, { "x", Slot::Type::stINPUT, Slot::ValueType::svFLOAT });
	pNodeGraph->AddSlot(this, { "y", Slot::Type::stINPUT, Slot::ValueType::svFLOAT });
	pNodeGraph->AddSlot(this, { "z", Slot::Type::stINPUT, Slot::ValueType::svFLOAT });
	pNodeGraph->AddSlot(this, { "w", Slot::Type::stINPUT, Slot::ValueType::svFLOAT });

	pNodeGraph->AddSlot(this, { "vector", Slot::Type::stOUTPUT, Slot::ValueType::svVECTOR });
}

void VectorNode::Update(double deltaSeconds)
{
	for (uint32_t i = 0; i < inputs.size(); i++)
	{
		if (inputs[i].pCorrespond)
		{
			vector[i] = data[inputs[i].valueKey];
		}
	}

	data["vector"] = vector;
}

JSON VectorNode::ToJson() const
{
	JSON json;

	json["vector"] = vector;

	return json;
}

void VectorNode::FromJson(const JSON& json)
{
	vector = json["vector"];
}

void VectorNode::DrawWidget()
{
	imnodes::BeginNode(GetUID(), "Vector", { 237, 199, 49, 255 });
	{
		for (auto& input : inputs)
		{
			imnodes::InputAttribute(input.GetUID(), input.valueKey.c_str());
		}

		imnodes::OutputAttribute(outputs[0].GetUID(), outputs[0].valueKey.c_str());
	}
	imnodes::EndNode();
}