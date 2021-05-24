#pragma once
#include "node.h"

class NodeGraph
{
public:
	NodeBase* AddNode(const std::string& stringID);
	void AddSlot(NodeBase* pNode, Slot slot);

	void Reset();

	void SyncNodes(double deltaSeconds);

	JSON ToJson() const;
	void FromJson(const JSON& json);

	void DrawEditor();

private:
	void SyncNode(NodeBase* pNode, double deltaSeconds);
	Slot* FindSlot(int iSlotUID);
	void LinkNodes();

private:
	// global unique ids for imnodes
	int nodeGUID = 0;
	int slotGUID = 0;

	std::vector<NodeBase*> nodePtrs;
	// bool is for checking sync every frame, ImVec2 for gui draw position
	std::unordered_map<NodeBase*, std::tuple<bool, ImVec2>> nodeStates;

	std::vector<std::pair<int, int>> links;
};