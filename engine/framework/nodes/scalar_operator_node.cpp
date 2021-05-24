#include "scalar_operator_node.h"
#include "node_graph.h"

const std::vector<std::string> ScalarOperatorNode::operationNames = {
	"add",
	"subtract",
	"multiply",
	"devide"
};

void ScalarOperatorNode::InitializeSlots()
{
	pNodeGraph->AddSlot(this, { "lhs", Slot::Type::stINPUT, Slot::ValueType::svFLOAT });
	pNodeGraph->AddSlot(this, { "rhs", Slot::Type::stINPUT, Slot::ValueType::svFLOAT });

	pNodeGraph->AddSlot(this, { "result", Slot::Type::stOUTPUT, Slot::ValueType::svFLOAT });
}

void ScalarOperatorNode::Update(double deltaSeconds)
{
	switch (operationIndex)
	{
	case 0:
		data[outputs[0].valueKey] = data.value(inputs[0].valueKey, 0.0f) + data.value(inputs[1].valueKey, 0.0f);
		break;
	case 1:
		data[outputs[0].valueKey] = data.value(inputs[0].valueKey, 0.0f) - data.value(inputs[1].valueKey, 0.0f);
		break;
	case 2:
		data[outputs[0].valueKey] = data.value(inputs[0].valueKey, 0.0f) * data.value(inputs[1].valueKey, 0.0f);
		break;
	case 3:
		data[outputs[0].valueKey] = data.value(inputs[0].valueKey, 0.0f) / data.value(inputs[1].valueKey, 0.0f);
		break;
	}
}

JSON ScalarOperatorNode::ToJson() const
{
	JSON json;

	json["operationIndex"] = operationIndex;

	return json;
}

void ScalarOperatorNode::FromJson(const JSON& json)
{
	operationIndex = json["operationIndex"];
}

void ScalarOperatorNode::DrawWidget()
{
	imnodes::BeginNode(GetUID(), "Scalar Operation", { 102, 49, 159, 255 });
	{
		ImGui::SetNextItemWidth(imnodes::defaultNodeWidth - ImGui::CalcTextSize("operation").x);
		if (ImGui::BeginCombo("operation", operationNames[operationIndex].c_str()))
		{
			for (uint32_t i = 0; i < operationNames.size(); i++)
			{
				if (ImGui::Selectable(operationNames[i].c_str()))
				{
					operationIndex = i;
				}
			}

			ImGui::EndCombo();
		}

		imnodes::InputAttribute(inputs[0].GetUID(), inputs[0].valueKey.c_str());
		imnodes::InputAttribute(inputs[1].GetUID(), inputs[0].valueKey.c_str());

		imnodes::OutputAttribute(outputs[0].GetUID(), outputs[0].valueKey.c_str());
	}
	imnodes::EndNode();
}