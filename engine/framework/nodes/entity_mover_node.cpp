#include "entity_mover_node.h"
#include "node_graph.h"
#include "entity.h"
#include "components/transform_component.h"

void EntityMoverNode::InitializeSlots()
{
	pNodeGraph->AddSlot(this, { "entity", Slot::Type::stINPUT, Slot::ValueType::svADDRESS });
	pNodeGraph->AddSlot(this, { "position", Slot::Type::stINPUT, Slot::ValueType::svVECTOR });
	pNodeGraph->AddSlot(this, { "condition", Slot::Type::stINPUT, Slot::ValueType::svBOOL });
}

void EntityMoverNode::Update(double deltaSeconds)
{
	// condition satisfied
	if (data.value(inputs[2].valueKey, false))
	{
		// entity found
		if (auto pEntity = reinterpret_cast<Entity*>(data.value(inputs[0].valueKey, (void*)nullptr)))
		{
			// entity has transform component
			if (auto pTrasnform = pEntity->GetComponent<TransformComponent>())
			{
				pTrasnform->SetAbsolutePosition(data[inputs[1].valueKey]);
			}
		}
	}
}

void EntityMoverNode::DrawWidget()
{
	imnodes::BeginNode(GetUID(), "Move Entity", { 150, 55, 255, 255 });
	{
		for (auto& input : inputs)
		{
			imnodes::InputAttribute(input.GetUID(), input.valueKey.c_str());
		}
	}
	imnodes::EndNode();
}