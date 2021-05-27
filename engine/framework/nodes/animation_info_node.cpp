#include "animation_info_node.h"
#include "node_graph.h"
#include "entity.h"
#include "components/model_component.h"
#include "rendering/animator.h"

void AnimationInfoNode::InitializeSlots()
{
	pNodeGraph->AddSlot(this, { "entity", Slot::Type::stINPUT, Slot::ValueType::svADDRESS });

	pNodeGraph->AddSlot(this, { "currentAnimation", Slot::Type::stOUTPUT, Slot::ValueType::svSTRING });
	pNodeGraph->AddSlot(this, { "isLooping", Slot::Type::stOUTPUT, Slot::ValueType::svBOOL });
	pNodeGraph->AddSlot(this, { "isFinished", Slot::Type::stOUTPUT, Slot::ValueType::svBOOL });
}

void AnimationInfoNode::Update(double deltaSeconds)
{
	if (auto pEntity = reinterpret_cast<Entity*>(data.value(inputs[0].valueKey, (void*)nullptr)))
	{
		if (auto pModelComponent = pEntity->GetComponent<ModelComponent>())
		{
			auto pAnimator = pModelComponent->GetAnimator();
			data[outputs[0].valueKey] = pAnimator ? pAnimator->GetCurrentAnimation()->GetName() : std::string{};
			data[outputs[1].valueKey] = pAnimator ? pAnimator->GetCurrentAnimation()->IsLooping() : false;
			data[outputs[2].valueKey] = pAnimator ? pAnimator->GetCurrentAnimation()->IsFinished() : false;
		}
	}
}

void AnimationInfoNode::DrawWidget()
{
	imnodes::BeginNode(GetUID(), "Animation Info", { 74, 128, 48, 255 });
	{
		imnodes::InputAttribute(inputs[0].GetUID(), inputs[0].valueKey.c_str());

		imnodes::OutputAttribute(outputs[0].GetUID(), outputs[0].valueKey.c_str());
		imnodes::OutputAttribute(outputs[1].GetUID(), outputs[1].valueKey.c_str());
		imnodes::OutputAttribute(outputs[2].GetUID(), outputs[2].valueKey.c_str());
	}
	imnodes::EndNode();
}