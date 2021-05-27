#pragma once
#include "engine_gui.h"
#include "entity.h"

class Scene;

class EntityInspector : public EngineGUI
{
private:
	uint32_t componentIndex = 0;

public:
	void Draw() override;

private:
	void EditEntityName(Entity* pEntity);
	void ShowEntityInfo(Entity* pEntity);
	void EditEntityPedigree(Scene* pScene, Entity*& pEntity);
	void EditEntityComponents(Entity* pEntity);
	void DrawEntityComponentWidgets(Entity* pEntity);
};