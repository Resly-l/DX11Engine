#pragma once
#include "common.h"

class EngineGUI
{
public:
	virtual void Draw() = 0;

protected:
	inline static const int windowFlags = ImGuiWindowFlags_NoCollapse;
};