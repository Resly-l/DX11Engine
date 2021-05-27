#include "node_factory.h"
#include "nodes/boolean_node.h"
#include "nodes/logical_operator_node.h"
#include "nodes/counter_node.h"
#include "nodes/event_listener_node.h"
#include "nodes/event_emitter_node.h"
#include "nodes/branch_node.h"
#include "nodes/scalar_node.h"
#include "nodes/scalar_operator_node.h"
#include "nodes/string_node.h"
#include "nodes/string_operator_node.h"
#include "nodes/vector_node.h"
#include "nodes/vector_copier_node.h"
#include "nodes/vector_decomposer_node.h"
#include "nodes/vector_operator_node.h"
#include "nodes/keyboard_node.h"
#include "nodes/mouse_node.h"
#include "nodes/entity_finder_node.h"
#include "nodes/entity_mover_node.h"
#include "nodes/transform_modifier_node.h"
#include "nodes/sphere_confiner_node.h"
#include "nodes/observer_node.h"
#include "nodes/animation_changer_node.h"
#include "nodes/animation_info_node.h"
#include "nodes/gravity_modifier_node.h"

void NodeFactory::Initialize()
{
	Register<BooleanNode>();
	Register<LogicalOperatorNode>();
	Register<CounterNode>();
	Register<EventListenerNode>();
	Register<EventEmitterNode>();
	Register<BranchNode>();
	Register<ScalarNode>();
	Register<ScalarOperatorNode>();
	Register<StringNode>();
	Register<StringOperatorNode>();
	Register<VectorNode>();
	Register<VectorCopierNode>();
	Register<VectorDecomposerNode>();
	Register<VectorOperatorNode>();
	Register<KeyboardNode>();
	Register<MouseNode>();
	Register<EntityFinderNode>();
	Register<EntityMoverNode>();
	Register<TransformModifierNode>();
	Register<SphereConfinerNode>();
	Register<ObserverNode>();
	Register<AnimationChangerNode>();
	Register<AnimationInfoNode>();
	Register<GravityModifierNode>();
}