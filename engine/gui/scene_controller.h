#pragma once
#include "engine_gui.h"
#include "rendering/texture.h"

class SceneController : public EngineGUI
{
private:
	Texture playIcon;
	Texture pauseIcon;
	Texture stopIcon;

public:
	SceneController();

public:
	void Draw() override;
};