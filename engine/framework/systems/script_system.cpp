#include "script_system.h"
#include "components/script_component.h"
#include "component_factory.h"

void ScriptSystem::Update(double deltaSeconds)
{
	if (deltaSeconds == 0.0f)
	{
		return;
	}

	for (auto& pScriptComponent : ComponentFactory::GetInstances(ScriptComponent::ID))
	{
		static_cast<ScriptComponent*>(pScriptComponent.get())->Execute(deltaSeconds);
	}
}