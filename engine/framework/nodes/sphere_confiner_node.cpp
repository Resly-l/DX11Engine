#include "sphere_confiner_node.h"
#include "node_graph.h"
#include "entity.h"
#include "components/transform_component.h"

void SphereConfinerNode::InitializeSlots()
{
	pNodeGraph->AddSlot(this, { "entity", Slot::Type::stINPUT, Slot::ValueType::svADDRESS });
	pNodeGraph->AddSlot(this, { "target", Slot::Type::stINPUT, Slot::ValueType::svADDRESS });
}

void SphereConfinerNode::Update(double deltaSeconds)
{
	auto pEntity = reinterpret_cast<Entity*>(data.value(inputs[0].valueKey, (void*)nullptr));
	auto pTarget = reinterpret_cast<Entity*>(data.value(inputs[1].valueKey, (void*)nullptr));

	if (pEntity && pTarget)
	{
		auto pEntityTransform = pEntity->GetComponent<TransformComponent>();
		auto pTargetTransform = pTarget->GetComponent<TransformComponent>();

		if (pEntityTransform && pTargetTransform)
		{
			const Vector entityAbsolutePosition = pEntityTransform->GetAbsolutePosition();
			const Vector targetAbsolutePosition = pTargetTransform->GetAbsolutePosition();
			const Vector offsetFromTarget = (entityAbsolutePosition - targetAbsolutePosition).GetNormalized3() * radius;

			pEntityTransform->SetAbsolutePosition(offsetFromTarget + targetAbsolutePosition);
			pEntityTransform->LookAt(targetAbsolutePosition);
		}
	}
}

JSON SphereConfinerNode::ToJson() const
{
	JSON json;

	json["radius"] = radius;

	return json;
}

void SphereConfinerNode::FromJson(const JSON& json)
{
	radius = json["radius"];
}

void SphereConfinerNode::DrawWidget()
{
	imnodes::BeginNode(GetUID(), "Sphere Confiner", { 60, 120, 70, 255 });
	{
		ImGui::SetNextItemWidth(imnodes::defaultNodeWidth - ImGui::CalcTextSize("radius").x);
		ImGui::InputFloat("radius", &radius);

		imnodes::InputAttribute(inputs[0].GetUID(), inputs[0].valueKey.c_str());
		imnodes::InputAttribute(inputs[1].GetUID(), inputs[1].valueKey.c_str());
	}
	imnodes::EndNode();
}