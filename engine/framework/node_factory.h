#pragma once
#include "factory.h"
#include "node.h"

class NodeFactory : public Factory<NodeID, NodeBase>
{
public:
	static void Initialize();
};