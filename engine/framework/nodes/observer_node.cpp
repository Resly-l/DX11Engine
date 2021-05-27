#include "observer_node.h"
#include "node_graph.h"
#include "entity.h"
#include "components/transform_component.h"

void ObserverNode::InitializeSlots()
{
	pNodeGraph->AddSlot(this, { "observer", Slot::Type::stINPUT, Slot::ValueType::svADDRESS });
	pNodeGraph->AddSlot(this, { "target", Slot::Type::stINPUT, Slot::ValueType::svADDRESS });
	pNodeGraph->AddSlot(this, { "condition", Slot::Type::stINPUT, Slot::ValueType::svBOOL });
}

void ObserverNode::Update(double deltaSeconds)
{
	if (data.value(inputs[2].valueKey, false) == false)
	{
		return;
	}

	auto pLhs = reinterpret_cast<Entity*>(data.value(inputs[0].valueKey, (void*)nullptr));
	auto pRhs = reinterpret_cast<Entity*>(data.value(inputs[1].valueKey, (void*)nullptr));

	if (pLhs && pRhs)
	{
		auto pLhsTransform = pLhs->GetComponent<TransformComponent>();
		auto pRhsTransform = pRhs->GetComponent<TransformComponent>();

		if (pLhsTransform && pRhsTransform)
		{
			pLhsTransform->LookAt(pRhsTransform->GetAbsoluteTransform()[3]);
		}
	}
}

void ObserverNode::DrawWidget()
{
	imnodes::BeginNode(GetUID(), "Observe", { 40, 40, 40, 255 });
	{
		imnodes::InputAttribute(inputs[0].GetUID(), inputs[0].valueKey.c_str());
		imnodes::InputAttribute(inputs[1].GetUID(), inputs[1].valueKey.c_str());
		imnodes::InputAttribute(inputs[2].GetUID(), inputs[2].valueKey.c_str());
	}
	imnodes::EndNode();
}