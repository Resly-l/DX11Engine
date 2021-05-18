#pragma once
#include "node.h"

class NodeGraph
{
private:
	// global unique ids for imnodes
	int iNodeGUID = 0;
	int iSlotGUID = 0;

	std::vector<NodeBase*> nodePtrs;
	// bool is for checking sync every frame, ImVec2 for gui draw position
	std::unordered_map<NodeBase*, std::tuple<bool, ImVec2>> nodeStates;

	std::vector<std::pair<int, int>> links;

public:
	NodeBase* AddNode(const std::string& stringID);
	void AddSlot(NodeBase* pNode, Slot slot);

	void SyncNodes(double deltaSeconds);

	JSON ToJson() const;
	void FromJson(const JSON& json);

	void DrawEditor();

private:
	void SyncNode(NodeBase* pNode, double deltaSeconds);
};