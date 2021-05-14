#pragma once
#include "engine_gui.h"
#include "system.h"

class SystemViewer : public EngineGUI
{
private:
	std::unordered_map<SystemID, std::string> systemNames;

public:
	SystemViewer();

public:
	void Draw() override;
};