#include "scene.h"
#include "entity.h"
#include "system.h"

#include "systems/render_system.h"
#include "components/camera_component.h"

Scene::Scene()
	:
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

void Scene::Play(double deltaSeconds)
{
	// camera entity found;
	if (auto pCamera = FindEntity(cameraName))
	{
		CameraComponent* pCameraComponent = pCamera->GetComponent<CameraComponent>();

		// if camera entity doesn't have camera component
		if (pCameraComponent == nullptr)
		{
			// assign one
			pCameraComponent = pCamera->AssignComponent<CameraComponent>();
		}

		SystemContainer::GetSystem<RenderSystem>()->SetCamera(pCameraComponent);
	}
	else
	{
		SystemContainer::GetSystem<RenderSystem>()->SetCamera(nullptr);
	}

	for (auto& pSystem : SystemContainer::GetSystems())
	{
		pSystem->Update(deltaSeconds);
	}
}

SceneManager SceneManager::singleton;

bool SceneManager::SetActiveScene(const std::string& sceneName)
{
	if (auto it = singleton.scenePtrs.find(sceneName); it != singleton.scenePtrs.end())
	{
		singleton.pActiveScene = it->second.get();
		return true;
	}

	return false;
}

void SceneManager::AddScene(std::unique_ptr<Scene> pScene)
{
	singleton.scenePtrs[pScene->GetName()] = std::move(pScene);
}