#include "animation_changer_node.h"
#include "node_graph.h"
#include "entity.h"
#include "components/model_component.h"
#include "rendering/animator.h"

void AnimationChangerNode::InitializeSlots()
{
	pNodeGraph->AddSlot(this, { "entity", Slot::Type::stINPUT, Slot::ValueType::svADDRESS });
	pNodeGraph->AddSlot(this, { "loop", Slot::Type::stINPUT, Slot::ValueType::svBOOL });
	pNodeGraph->AddSlot(this, { "condition", Slot::Type::stINPUT, Slot::ValueType::svBOOL });
}

void AnimationChangerNode::Update(double deltaSeconds)
{
	if (data[inputs[2].valueKey] == false)
	{
		return;
	}

	if (auto pEntity = reinterpret_cast<Entity*>(data.value(inputs[0].valueKey, (void*)nullptr)))
	{
		if (auto pModelComponent = pEntity->GetComponent<ModelComponent>())
		{
			if (auto pAnimator = pModelComponent->GetAnimator())
			{
				if (pAnimator->SetAnimation(animationName))
				{
					pAnimator->GetCurrentAnimation()->SetLoop(data[inputs[1].valueKey]);
				}
			}
		}
	}
}

JSON AnimationChangerNode::ToJson() const
{
	JSON json;

	json["animationName"] = animationName;

	return json;
}

void AnimationChangerNode::FromJson(const JSON& json)
{
	animationName = json["animationName"];
}

void AnimationChangerNode::DrawWidget()
{
	imnodes::BeginNode(GetUID(), "Change Animation", { 200, 100, 100, 255 });
	{
		ImGui::SetNextItemWidth(imnodes::defaultNodeWidth - ImGui::CalcTextSize("animation name").x);
		ImGui::InputText("animation name", &animationName);

		imnodes::InputAttribute(inputs[0].GetUID(), inputs[0].valueKey.c_str());
		imnodes::InputAttribute(inputs[1].GetUID(), inputs[1].valueKey.c_str());
		imnodes::InputAttribute(inputs[2].GetUID(), inputs[2].valueKey.c_str());
	}
	imnodes::EndNode();
}