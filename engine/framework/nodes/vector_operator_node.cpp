#include "vector_operator_node.h"
#include "node_graph.h"

const std::vector<std::string> VectorOperatorNode::operationNames = {
	"normalize",
	"invert",
	"multiply",
	"devide",
	"add",
	"subtract",
	"dot",
	"cross",
	"rotate",
	"lerp"
};

void VectorOperatorNode::InitializeSlots()
{
	pNodeGraph->AddSlot(this, { "lhs", Slot::Type::stINPUT, Slot::ValueType::svVECTOR });
	pNodeGraph->AddSlot(this, { "rhs", Slot::Type::stINPUT, Slot::ValueType::svVECTOR });
	pNodeGraph->AddSlot(this, { "scalar", Slot::Type::stINPUT, Slot::ValueType::svFLOAT });

	pNodeGraph->AddSlot(this, { "result", Slot::Type::stOUTPUT, Slot::ValueType::svVECTOR });
}

void VectorOperatorNode::Update(double deltaSeconds)
{
	switch (operationIndex)
	{
	case 0:
		data[outputs[0].valueKey] = data.value(inputs[0].valueKey, Vector()).GetNormalized3();
		break;
	case 1:
		data[outputs[0].valueKey] = -data.value(inputs[0].valueKey, Vector());
		break;
	case 2:
		data[outputs[0].valueKey] = data.value(inputs[0].valueKey, Vector()) * data.value(inputs[1].valueKey, 0.0f);
		break;
	case 3:
		data[outputs[0].valueKey] = data.value(inputs[0].valueKey, Vector()) / data.value(inputs[1].valueKey, 0.0f);
		break;
	case 4:
		data[outputs[0].valueKey] = data.value(inputs[0].valueKey, Vector()) + data.value(inputs[1].valueKey, Vector());
		break;
	case 5:
		data[outputs[0].valueKey] = data.value(inputs[0].valueKey, Vector()) - data.value(inputs[1].valueKey, Vector());
		break;
	case 6:
		data[outputs[0].valueKey] = data.value(inputs[0].valueKey, Vector()).Dot3(data[inputs[1].valueKey]);
		break;
	case 7:
		data[outputs[0].valueKey] = data.value(inputs[0].valueKey, Vector()).Cross3(data[inputs[1].valueKey]);
		break;
	case 8:
		data[outputs[0].valueKey] = data.value(inputs[0].valueKey, Vector()) * Matrix::Rotation(data[inputs[1].valueKey]);
		break;
	case 9:
		data[outputs[0].valueKey] = data.value(inputs[0].valueKey, Vector()).Lerp(data[inputs[1].valueKey], data[inputs[2].valueKey]);
		break;
	}
}

JSON VectorOperatorNode::ToJson() const
{
	JSON json;

	json["operationIndex"] = operationIndex;

	return json;
}

void VectorOperatorNode::FromJson(const JSON& json)
{
	operationIndex = json["operationIndex"];
}

void VectorOperatorNode::DrawWidget()
{
	imnodes::BeginNode(GetUID(), "Vector Operation", { 136, 220, 170, 255 });
	{
		ImGui::SetNextItemWidth(imnodes::defaultNodeWidth - ImGui::CalcTextSize("operation").x);
		if (ImGui::BeginCombo("operation", operationNames[operationIndex].c_str()))
		{
			for (uint32_t i = 0; i < operationNames.size(); i++)
			{
				if (ImGui::Selectable(operationNames[i].c_str()))
				{
					operationIndex = i;

					for (auto& input : inputs)
					{
						input.RemoveConnection();
					}
				}
			}

			ImGui::EndCombo();
		}

		imnodes::InputAttribute(inputs[0].GetUID(), inputs[0].valueKey.c_str());
		if (operationIndex >= 4)
		{
			imnodes::InputAttribute(inputs[1].GetUID(), inputs[1].valueKey.c_str());
		}
		if (operationIndex == 2 || operationIndex == 3 || operationIndex == 9)
		{
			imnodes::InputAttribute(inputs[2].GetUID(), inputs[2].valueKey.c_str());
		}

		imnodes::OutputAttribute(outputs[0].GetUID(), outputs[0].valueKey.c_str());
	}
	imnodes::EndNode();
}