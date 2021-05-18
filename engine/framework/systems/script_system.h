#pragma once
#include "system.h"

class ScriptSystem : public System<ScriptSystem>
{
public:
	void Update(double deltaSeconds) override;
};