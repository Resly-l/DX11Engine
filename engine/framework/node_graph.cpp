#include "node_graph.h"
#include "node_factory.h"

NodeBase* NodeGraph::AddNode(const std::string& stringID)
{
	if (auto pNode = NodeFactory::Create(stringID))
	{
		pNode->pNodeGraph = this;
		pNode->uid = nodeGUID++;

		pNode->InitializeSlots();

		nodePtrs.push_back(pNode);
		nodeStates[pNode];

		return pNode;
	}

	return nullptr;
}

void NodeGraph::AddSlot(NodeBase* pNode, Slot slot)
{
	slot.uid = slotGUID++;
	slot.pOwner = pNode;

	if (slot.type == Slot::Type::stINPUT)
	{
		pNode->inputs.push_back(slot);
	}
	else
	{
		pNode->outputs.push_back(slot);
	}

	switch (slot.valueType)
	{
	case Slot::ValueType::svBOOL:
		pNode->data[slot.valueKey] = false;
		break;
	case Slot::ValueType::svFLOAT:
		pNode->data[slot.valueKey] = 0.0f;
		break;
	case Slot::ValueType::svVECTOR:
		pNode->data[slot.valueKey] = Vector();
		break;
	case Slot::ValueType::svADDRESS:
		pNode->data[slot.valueKey] = (void*)nullptr;
		break;
	}
}

void NodeGraph::SyncNodes(double deltaSeconds)
{
	for (auto pNode : nodePtrs)
	{
		SyncNode(pNode, deltaSeconds);
	}

	for (auto& [pNode, tuple] : nodeStates)
	{
		// setting sync flag to false
		std::get<bool>(tuple) = false;
	}
}

JSON NodeGraph::ToJson() const
{
	JSON json;

	for (auto& [pNode, tuple] : nodeStates)
	{
		auto nodeJson = pNode->ToJson();
		nodeJson["string_id"] = pNode->GetStringID();

		json["nodes"].push_back(nodeJson);

		JSON positionJson;

		positionJson["x"] = std::get<ImVec2>(tuple).x;
		positionJson["y"] = std::get<ImVec2>(tuple).y;

		json["node_positions"].push_back(positionJson);
	}

	for (auto& link : links)
	{
		json["links"].push_back(link);
	}

	return json;
}

void NodeGraph::FromJson(const JSON& json)
{
	if (json.contains("nodes"))
	{
		for (auto& nodeJson : json["nodes"])
		{
			std::string stringID = nodeJson["string_id"];
			auto pNode = NodeFactory::Create(stringID);
			pNode->FromJson(nodeJson);

			pNode->pNodeGraph = this;
			pNode->uid = nodeGUID++;

			pNode->InitializeSlots();

			nodePtrs.push_back(pNode);
		}
	}

	if (json.contains("node_positions"))
	{
		uint32_t uNode = 0;
		for (auto& positionJson : json["node_positions"])
		{
			auto& position = std::get<ImVec2>(nodeStates[nodePtrs[uNode++]]);

			position.x = positionJson["x"];
			position.y = positionJson["y"];
		}
	}

	if (json.contains("links"))
	{
		for (auto& linkJson : json["links"])
		{
			links.push_back(linkJson);
		}
	}
}

void NodeGraph::SyncNode(NodeBase* pNode, double deltaSeconds)
{
	for (auto& input : pNode->inputs)
	{
		// if input is connected to dependency's(other node's) output
		if (input.pCorrespond)
		{
			auto pDependency = input.pCorrespond->pOwner;

			// if dependency wasn't updated
			if (std::get<bool>(nodeStates[pDependency]) == false)
			{
				SyncNode(pDependency, deltaSeconds);
			}
		}
	}

	pNode->Update(deltaSeconds);

	// setting sync flag to true
	std::get<bool>(nodeStates[pNode]) = true;
}

Slot* NodeGraph::FindSlot(int iSlotUID)
{
	for (auto& pNode : nodePtrs)
	{
		for (auto& input : pNode->inputs)
		{
			if (input.uid == iSlotUID) return &input;
		}
		for (auto& output : pNode->outputs)
		{
			if (output.uid == iSlotUID) return &output;
		}
	}

	return nullptr;
}

void NodeGraph::DrawEditor()
{
	ImGui::BeginChildFrame(ImGui::GetID("node_editor"), {});
	imnodes::BeginNodeEditor();
	{
		// draw node widgets
		{
			for (int iNode = 0; iNode < nodePtrs.size(); iNode++)
			{
				const ImVec2& preposition = imnodes::GetNodeEditorSpacePos(iNode);
				ImVec2& nodePosition = std::get<ImVec2>(nodeStates[nodePtrs[iNode]]);

				if (!ImGui::IsMouseDragging(ImGuiMouseButton_Left))
				{
					if (preposition.x != nodePosition.x || preposition.y != nodePosition.y)
					{
						imnodes::SetNodeGridSpacePos(iNode, nodePosition);
					}
				}

				nodePtrs[iNode]->DrawWidget();

				nodePosition = imnodes::GetNodeGridSpacePos(iNode);
			}
		}

		// draw links
		for (int iLink = 0; iLink < links.size(); iLink++)
		{
			imnodes::Link(iLink, links[iLink].first, links[iLink].second);
		}

		if (imnodes::IsEditorHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup("node_addition");
		}

		if (ImGui::BeginPopup("node_addition"))
		{
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Nodes");
			ImGui::Separator();

			for (auto& stringID : NodeFactory::GetRegisteredStringIDs())
			{
				if (ImGui::Button(stringID.c_str()))
				{
					auto pNode = AddNode(stringID);
					std::get<ImVec2>(nodeStates[pNode]) = ImGui::GetCursorPos();
				}
			}

			ImGui::EndPopup();
		}
	}
	imnodes::EndNodeEditor();
	ImGui::EndChildFrame();

	LinkNodes();
}

void NodeGraph::LinkNodes()
{
	int iStartID, iEndID;
	if (imnodes::IsLinkCreated(&iStartID, &iEndID))
	{
		auto pStartSlot = FindSlot(iStartID);
		auto pEndSlot = FindSlot(iEndID);

		if (pStartSlot && pEndSlot)
		{
			pStartSlot->ConnectTo(pEndSlot);
			links.push_back({ iStartID, iEndID });
		}
	}
}