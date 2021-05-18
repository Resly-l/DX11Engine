#include "scene_manager.h"
#include "file_manager.h"

#include "system_container.h"
#include "systems/render_system.h"
#include "components/camera_component.h"

SceneManager SceneManager::singleton;

void SceneManager::Play(double deltaSeconds)
{
	if (singleton.pActiveScene == nullptr)
	{
		return;
	}

	if (auto pEntity = singleton.pActiveScene->FindEntity(singleton.pActiveScene->GetCameraName()))
	{
		CameraComponent* pCameraComponent = pEntity->GetComponent<CameraComponent>();

		if (pCameraComponent == nullptr)
		{
			pCameraComponent = pEntity->AssignComponent<CameraComponent>();
		}

		SystemContainer::GetSystem<RenderSystem>()->SetCamera(pCameraComponent);
	}
	else
	{
		SystemContainer::GetSystem<RenderSystem>()->SetCamera(nullptr);
	}

	for (auto& pSystem : SystemContainer::GetSystems())
	{
		pSystem->Update(deltaSeconds * singleton.fDeltatimeFactor);
	}
}

void SceneManager::SetSceneState(Scene::State state)
{
	if (singleton.pActiveScene == nullptr)
	{
		return;
	}

	switch (state)
	{
	case Scene::State::ssIDLE:
	{
		if (auto json = FileManager::ReadFile(temporarySavePath); json.is_null() == false)
		{
			singleton.pActiveScene->FromJson(json);
			FileManager::DeleteFileFromPath(temporarySavePath);
			Console::AddLog({ Log::Type::ltNOTIFICATION, "scene loaded from temporary file" });
		}

		singleton.fDeltatimeFactor = 0.0f;

		break;
	}
	case Scene::State::ssPLAYING:
	{
		// only save if previous state was idle to prevent saving while playing
		if (singleton.sceneState == Scene::State::ssIDLE)
		{
			SaveSceneToFile(singleton.pActiveScene->GetName(), temporarySavePath);
			Console::AddLog({ Log::Type::ltNOTIFICATION, "scene saved to temporary file" });
		}

		singleton.fDeltatimeFactor = 1.0f;
		break;
	}
	case Scene::State::ssPAUSED:
	{
		singleton.fDeltatimeFactor = 0.0f;
		break;
	}
	}

	singleton.sceneState = state;
}

bool SceneManager::SetActiveScene(const std::string& sceneName)
{
	if (auto it = singleton.scenePtrs.find(sceneName); it != singleton.scenePtrs.end())
	{
		singleton.pActiveScene = it->second.get();
		singleton.sceneState = Scene::State::ssIDLE;
		return true;
	}

	return false;
}

void SceneManager::AddScene(std::unique_ptr<Scene> pScene)
{
	singleton.scenePtrs[pScene->GetName()] = std::move(pScene);
}

bool SceneManager::RemoveScene(const std::string& sceneName)
{
	if (auto it = singleton.scenePtrs.find(sceneName); it != singleton.scenePtrs.end())
	{
		singleton.scenePtrs.erase(it);
		return true;
	}

	return false;
}

bool SceneManager::RemoveScene(Scene* pTarget)
{
	return std::erase_if(singleton.scenePtrs,
		[pTarget](const std::pair<const std::string, std::unique_ptr<Scene>>& pair)
		{
			return pair.second.get() == pTarget;
		}) != 0;
}

bool SceneManager::SaveSceneToFile(const std::string& sceneName, const std::string& filePath)
{
	if (auto it = singleton.scenePtrs.find(sceneName); it != singleton.scenePtrs.end())
	{
		return FileManager::CreateFileToPath(filePath, it->second->ToJson());
	}

	return false;
}

bool SceneManager::LoadSceneFromFile(const std::string& filePath)
{
	if (auto json = FileManager::ReadFile(filePath); json.is_null() == false)
	{
		auto pScene = std::make_unique<Scene>();
		pScene->FromJson(json);
		AddScene(std::move(pScene));
	}
	return false;
}

bool SceneManager::SwapScene(const std::string& sceneName)
{
	if (auto it = singleton.scenePtrs.find(sceneName); it != singleton.scenePtrs.end())
	{
		singleton.pActiveScene = it->second.get();
		singleton.fDeltatimeFactor = 1.0f;
		singleton.sceneState = Scene::State::ssIDLE;
		return true;
	}

	return false;
}