#include "node_factory.h"
#include "nodes/event_listener_node.h"

void NodeFactory::Initialize()
{
	Register<EventListenerNode>();
}