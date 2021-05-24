#include "logical_operator_node.h"
#include "node_graph.h"

const std::vector<std::string> LogicalOperatorNode::operationNames = {
	"AND",
	"OR",
	"XOR"
};

void LogicalOperatorNode::InitializeSlots()
{
	pNodeGraph->AddSlot(this, { "lhs", Slot::Type::stINPUT, Slot::ValueType::svBOOL });
	pNodeGraph->AddSlot(this, { "rhs", Slot::Type::stINPUT, Slot::ValueType::svBOOL });

	pNodeGraph->AddSlot(this, { "result", Slot::Type::stOUTPUT, Slot::ValueType::svBOOL });
}

void LogicalOperatorNode::Update(double deltaSeconds)
{
	switch (operationIndex)
	{
	case 0:
		data[outputs[0].valueKey] = data.value(inputs[0].valueKey, false) & data.value(inputs[1].valueKey, false);
		break;
	case 1:
		data[outputs[0].valueKey] = data.value(inputs[0].valueKey, false) | data.value(inputs[1].valueKey, false);
		break;
	case 2:
		data[outputs[0].valueKey] = data.value(inputs[0].valueKey, false) ^ data.value(inputs[1].valueKey, false);
		break;
	}
}

JSON LogicalOperatorNode::ToJson() const
{
	JSON json;

	json["operationIndex"] = operationIndex;

	return json;
}

void LogicalOperatorNode::FromJson(const JSON& json)
{
	operationIndex = json["operationIndex"];
}

void LogicalOperatorNode::DrawWidget()
{
	imnodes::BeginNode(GetUID(), "Logical Operation", { 186, 89, 38, 255 });
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
		imnodes::InputAttribute(inputs[1].GetUID(), inputs[1].valueKey.c_str());

		imnodes::OutputAttribute(outputs[0].GetUID(), outputs[0].valueKey.c_str());
	}
	imnodes::EndNode();
}