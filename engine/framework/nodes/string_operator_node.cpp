#include "string_operator_node.h"
#include "node_graph.h"

const std::vector<std::string> StringOperatorNode::operationNames = {
	"compare"
};

void StringOperatorNode::InitializeSlots()
{
	pNodeGraph->AddSlot(this, { "lhs", Slot::Type::stINPUT, Slot::ValueType::svSTRING });
	pNodeGraph->AddSlot(this, { "rhs", Slot::Type::stINPUT, Slot::ValueType::svSTRING });

	pNodeGraph->AddSlot(this, { "result", Slot::Type::stOUTPUT, Slot::ValueType::svBOOL });
}

void StringOperatorNode::Update(double deltaSeconds)
{
	switch (operationIndex)
	{
	case 0:
		data[outputs[0].valueKey] = data.value(inputs[0].valueKey, std::string{}) == data.value(inputs[1].valueKey, std::string{});
		break;
	}
}

JSON StringOperatorNode::ToJson() const
{
	JSON json;

	json["operationIndex"] = operationIndex;

	return json;
}

void StringOperatorNode::FromJson(const JSON& json)
{
	operationIndex = json["operationIndex"];
}

void StringOperatorNode::DrawWidget()
{
	imnodes::BeginNode(GetUID(), "String Operator", { 200, 200, 255, 255 });
	{
		ImGui::SetNextItemWidth(imnodes::defaultNodeWidth - ImGui::CalcTextSize("operation").x);
		if (ImGui::BeginCombo("operation", operationNames[operationIndex].c_str()))
		{
			for (uint32_t i = 0; i < operationNames.size(); i++)
			{
				if (ImGui::Selectable(operationNames[operationIndex].c_str()))
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