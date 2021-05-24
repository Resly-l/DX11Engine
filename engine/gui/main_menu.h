#pragma once
#include "engine_gui.h"

class MainMenu : public EngineGUI
{
public:
	void Draw() override;

private:
	void CreateNewScene();
	void OpenSceneFromFile();
	void SaveSceneToFile(bool bSelectPath);

private:
	std::string savedScenePath;
};