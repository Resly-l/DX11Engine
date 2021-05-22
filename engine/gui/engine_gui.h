#pragma once
#include "common.h"

class EngineGUI
{
protected:
	inline static const int windowFlags = ImGuiWindowFlags_NoCollapse;

public:
	virtual void Draw() = 0;
};