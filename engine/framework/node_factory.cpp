#include "node_factory.h"
#include "nodes/counter_node.h"
#include "nodes/event_listener_node.h"
#include "nodes/event_emitter_node.h"
#include "nodes/branch_node.h"
#include "nodes/logical_operator_node.h"
#include "nodes/scalar_node.h"
#include "nodes/scalar_operator_node.h"
#include "nodes/vector_node.h"
#include "nodes/vector_operator_node.h"
#include "nodes/keyboard_node.h"
#include "nodes/mouse_node.h"
#include "nodes/entity_finder_node.h"
#include "nodes/entity_mover_node.h"
#include "nodes/transform_modifier_node.h"
#include "nodes/sphere_confiner_node.h"

void NodeFactory::Initialize()
{
	Register<CounterNode>();
	Register<EventListenerNode>();
	Register<EventEmitterNode>();
	Register<BranchNode>();
	Register<LogicalOperatorNode>();
	Register<ScalarNode>();
	Register<ScalarOperatorNode>();
	Register<VectorNode>();
	Register<VectorOperatorNode>();
	Register<KeyboardNode>();
	Register<MouseNode>();
	Register<EntityFinderNode>();
	Register<EntityMoverNode>();
	Register<TransformModifierNode>();
	Register<SphereConfinerNode>();
}