#include "gravity_modifier_node.h"
#include "node_graph.h"
#include "entity.h"
#include "components/physics_component.h"

void GravityModifierNode::InitializeSlots()
{
	pNodeGraph->AddSlot(this, { "entity", Slot::Type::stINPUT, Slot::ValueType::svADDRESS });
	pNodeGraph->AddSlot(this, { "factor", Slot::Type::stINPUT, Slot::ValueType::svFLOAT });
	pNodeGraph->AddSlot(this, { "condition", Slot::Type::stINPUT, Slot::ValueType::svBOOL });
}

void GravityModifierNode::Update(double deltaSeconds)
{
	if (data[inputs[2].valueKey] == false)
	{
		return;
	}

	if (auto pEntity = reinterpret_cast<Entity*>(data.value(inputs[0].valueKey, (void*)nullptr)))
	{
		if (auto pPhysicsComponent = pEntity->GetComponent<PhysicsComponent>())
		{
			pPhysicsComponent->SetGravityFactor(data[inputs[1].valueKey]);
		}
	}
}

void GravityModifierNode::DrawWidget()
{
	imnodes::BeginNode(GetUID(), "Modify Gravity", { 81, 0, 181, 255 });
	{
		imnodes::InputAttribute(inputs[0].GetUID(), inputs[0].valueKey.c_str());
		imnodes::InputAttribute(inputs[1].GetUID(), inputs[1].valueKey.c_str());
		imnodes::InputAttribute(inputs[2].GetUID(), inputs[2].valueKey.c_str());
	}
	imnodes::EndNode();
}