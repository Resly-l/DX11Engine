#include "component_factory.h"

#include "components/transform_component.h"
#include "components/render_component.h"
#include "components/camera_component.h"
#include "components/light_component.h"

void ComponentFactory::Initialize()
{
	Register<TransformComponent>();
	Register<RenderComponent>();
	Register<CameraComponent>();
	Register<LightComponent>();
}