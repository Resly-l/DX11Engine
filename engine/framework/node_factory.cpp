#include "node_factory.h"
#include "nodes/event_listener_node.h"
#include "nodes/event_emitter_node.h"

void NodeFactory::Initialize()
{
	Register<EventListenerNode>();
	Register<EventEmitterNode>();
}