#include "component_factory.h"

#include "components/transform_component.h"
#include "components/model_component.h"
#include "components/camera_component.h"
#include "components/light_component.h"
#include "components/script_component.h"

void ComponentFactory::Initialize()
{
	Register<TransformComponent>();
	Register<ModelComponent>();
	Register<CameraComponent>();
	Register<LightComponent>();
	Register<ScriptComponent>();
}