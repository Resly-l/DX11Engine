#pragma once
#include "engine_gui.h"
#include "rendering/texture.h"

class SceneController : public EngineGUI
{
public:
	SceneController();

public:
	void Draw() override;

private:
	Texture playIcon;
	Texture pauseIcon;
	Texture stopIcon;
};