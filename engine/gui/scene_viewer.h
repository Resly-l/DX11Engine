#pragma once
#include "engine_gui.h"

class Scene;
class Entity;

class SceneViewer : public EngineGUI
{
private:
	int treeFlags;

public:
	SceneViewer();

public:
	void Draw() override;

private:
	void DrawEntityHierarchy(Scene* pScene, Entity* pEntity, bool& bSelected);
	void EditSceneName(Scene* pScene);
	void EditCameraName(Scene* pScene);
};