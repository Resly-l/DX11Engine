#include "transform_modifier_node.h"
#include "node_graph.h"
#include "entity.h"
#include "components/transform_component.h"

void TransformModifierNode::InitializeSlots()
{
	pNodeGraph->AddSlot(this, { "entity", Slot::Type::stINPUT, Slot::ValueType::svADDRESS });

	pNodeGraph->AddSlot(this, { "position", Slot::Type::stINPUT, Slot::ValueType::svVECTOR });
	pNodeGraph->AddSlot(this, { "angle", Slot::Type::stINPUT, Slot::ValueType::svVECTOR });
	pNodeGraph->AddSlot(this, { "scale", Slot::Type::stINPUT, Slot::ValueType::svVECTOR });
	pNodeGraph->AddSlot(this, { "condition", Slot::Type::stINPUT, Slot::ValueType::svBOOL });

	pNodeGraph->AddSlot(this, { "position_out", Slot::Type::stOUTPUT, Slot::ValueType::svVECTOR });
	pNodeGraph->AddSlot(this, { "angle_out", Slot::Type::stOUTPUT, Slot::ValueType::svVECTOR });
	pNodeGraph->AddSlot(this, { "scale_out", Slot::Type::stOUTPUT, Slot::ValueType::svVECTOR });
	pNodeGraph->AddSlot(this, { "transform", Slot::Type::stOUTPUT, Slot::ValueType::svMATRIX });
}

void TransformModifierNode::Update(double deltaSeconds)
{
	if (auto pEntity = reinterpret_cast<Entity*>(data.value(inputs[0].valueKey, (void*)nullptr)))
	{
		if (auto pTransform = pEntity->GetComponent<TransformComponent>())
		{
			if (data.value(inputs[4].valueKey, false))
			{
				if (inputs[1].pCorrespond)
				{
					pTransform->SetAbsolutePosition(data[inputs[1].valueKey]);
				}
				if (inputs[2].pCorrespond)
				{
					pTransform->SetAbsoluteAngle(data[inputs[2].valueKey]);
				}
				if (inputs[3].pCorrespond)
				{
					pTransform->SetAbsoluteScale(data[inputs[3].valueKey]);
				}
			}

			const auto transform = pTransform->GetAbsoluteTransform();
			const auto decomposed = transform.Decompose();

			data[outputs[0].valueKey] = decomposed.position;
			data[outputs[1].valueKey] = decomposed.angle;
			data[outputs[2].valueKey] = decomposed.angle;
			data[outputs[3].valueKey] = transform;
		}
	}
}

void TransformModifierNode::DrawWidget()
{
	imnodes::BeginNode(GetUID(), "Modify Transform", { 64, 129, 248, 255 });
	{
		for (auto& input : inputs)
		{
			imnodes::InputAttribute(input.GetUID(), input.valueKey.c_str());
		}

		for (auto& output : outputs)
		{
			imnodes::OutputAttribute(output.GetUID(), output.valueKey.c_str());
		}
	}
	imnodes::EndNode();
}