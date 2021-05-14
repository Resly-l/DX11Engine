#pragma once
#include "engine_gui.h"
#include "entity.h"

class Scene;

class EntityInspector : public EngineGUI
{
private:
	std::vector<std::pair<std::string, std::tuple<ComponentID, std::string>>> componentIDs;
	uint32_t uComponentIndex = 0;

public:
	EntityInspector();

public:
	void Draw() override;

private:
	void EditEntityName(Entity* pEntity);
	void DrawEntityInfo(Entity* pEntity);
	void EditEntityPedigree(Scene* pScene, Entity*& pEntity);
	void EditEntityComponents(Entity* pEntity);
};