#include "script_system.h"
#include "components/script_component.h"
#include "component_factory.h"

void ScriptSystem::Update(double deltaSeconds, bool simulate)
{
	if (simulate == false)
	{
		return;
	}

	for (auto& pScriptComponent : ComponentFactory::GetInstances(ScriptComponent::ID))
	{
		static_cast<ScriptComponent*>(pScriptComponent.get())->Execute(deltaSeconds);
	}
}