#include "scene.h"
#include "entity.h"
#include "system.h"

static size_t uSceneCount = 0;

Scene::Scene()
	:
	name("unnamed_scene_#" + std::to_string(uSceneCount++)),
	pRootEntity(std::make_unique<Entity>())
{
	pRootEntity->SetName("Root_Entity");
}

Scene::Scene(const std::string& name)
	:
	name(name),
	pRootEntity(std::make_unique<Entity>())
{
	pRootEntity->SetName("Root_Entity");
}

Scene::~Scene() {}

Entity* Scene::FindEntity(const std::string& entityName) const
{
	return FindEntity(pRootEntity, entityName);
}

void Scene::SelectEntity(Entity* pEntity)
{
	pSelectedEntity = pEntity;
}

JSON Scene::ToJson() const
{
	JSON json;

	json["name"] = name;
	json["cameraName"] = cameraName;

	json["Root_Entity"] = pRootEntity->ToJson();

	return json;
}

void Scene::FromJson(const JSON& json)
{
	name = json["name"];
	cameraName = json["cameraName"];

	pRootEntity->FromJson(json["Root_Entity"]);
}

Entity* Scene::FindEntity(const std::unique_ptr<Entity>& pEntity, const std::string& entityName) const
{
	if (pEntity->GetName() == entityName)
	{
		return pEntity.get();
	}

	for (auto& pChild : pEntity->childPtrs)
	{
		if (auto pFound = FindEntity(pChild, entityName))
		{
			return pFound;
		}
	}

	return nullptr;
}