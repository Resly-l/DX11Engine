#pragma once
#include "factory.h"
#include "component.h"

class ComponentFactory : public Factory<ComponentID, ComponentBase>
{
public:
	static void Initialize();
};