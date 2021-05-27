#pragma once
#include "engine_gui.h"

class MainMenu : public EngineGUI
{
private:
	std::string savedScenePath;

public:
	void Draw() override;

private:
	void CreateNewScene();
	void OpenSceneFromFile();
	void SaveSceneToFile(bool bSelectPath);
};