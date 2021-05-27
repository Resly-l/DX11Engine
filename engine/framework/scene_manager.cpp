#include "scene_manager.h"
#include "file_manager.h"
#include "console.h"

#include "system_container.h"
#include "systems/render_system.h"
#include "components/camera_component.h"

SceneManager SceneManager::instance;

void SceneManager::Play(double deltaSeconds)
{
	if (instance.pActiveScene == nullptr)
	{
		return;
	}

	if (auto pEntity = instance.pActiveScene->FindEntity(instance.pActiveScene->GetCameraName()))
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
		pSystem->Update(deltaSeconds, instance.sceneState == Scene::State::ssPLAYING);
	}
}

void SceneManager::SetSceneState(Scene::State state)
{
	if (instance.pActiveScene == nullptr)
	{
		return;
	}

	switch (state)
	{
	case Scene::State::ssIDLE:
	{
		if (instance.sceneState == Scene::State::ssIDLE)
		{
			return;
		}

		if (auto json = FileManager::ReadFile(temporarySavePath); json.is_null() == false)
		{
			instance.pActiveScene->FromJson(json);
			FileManager::DeleteFileFromPath(temporarySavePath);
			Console::AddLog({ Log::Type::ltNOTIFICATION, "scene loaded from temporary file" });
		}

		break;
	}
	case Scene::State::ssPLAYING:
	{
		// only save if previous state was idle to prevent saving while playing
		if (instance.sceneState == Scene::State::ssIDLE)
		{
			SaveSceneToFile(instance.pActiveScene->GetName(), temporarySavePath);
			Console::AddLog({ Log::Type::ltNOTIFICATION, "scene saved to temporary file" });
		}

		break;
	}
	}

	instance.pActiveScene->SelectEntity(nullptr);
	instance.sceneState = state;
}

bool SceneManager::SetActiveScene(const std::string& sceneName)
{
	for (auto& pScene : instance.scenePtrs)
	{
		if (pScene->GetName() == sceneName)
		{
			instance.pActiveScene = pScene.get();
			instance.sceneState = Scene::State::ssIDLE;
			return true;
		}
	}

	return false;
}

bool SceneManager::SetActiveScene(Scene* pTargetScene)
{
	for (auto& pScene : instance.scenePtrs)
	{
		if (pScene.get() == pTargetScene)
		{
			instance.pActiveScene = pTargetScene;
			return true;
		}
	}

	return false;
}

Scene* SceneManager::AddScene(std::unique_ptr<Scene> pScene)
{
	instance.scenePtrs.push_back(std::move(pScene));
	return instance.scenePtrs.back().get();
}

bool SceneManager::RemoveScene(const std::string& sceneName)
{
	return std::erase_if(instance.scenePtrs,
		[&sceneName](const std::unique_ptr<Scene>& pScene)
		{
			return pScene->GetName() == sceneName;
		}) != 0;
}

bool SceneManager::RemoveScene(Scene* pTarget)
{
	return std::erase_if(instance.scenePtrs,
		[pTarget](const std::unique_ptr<Scene>& pScene)
		{
			return pScene.get() == pTarget;
		}) != 0;
}

void SceneManager::ClearScenes()
{
	instance.scenePtrs.clear();
	instance.pActiveScene = nullptr;
}

bool SceneManager::SaveSceneToFile(const std::string& sceneName, const std::string& filePath)
{
	auto it = std::find_if(instance.scenePtrs.begin(), instance.scenePtrs.end(),
		[&sceneName](const std::unique_ptr<Scene>& pScene)
		{
			return pScene->GetName() == sceneName;
		});

	if (it != instance.scenePtrs.end())
	{
		return FileManager::CreateFileToPath(filePath, it->get()->ToJson());
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
	auto it = std::find_if(instance.scenePtrs.begin(), instance.scenePtrs.end(),
		[&sceneName](const std::unique_ptr<Scene>& pScene)
		{
			return pScene->GetName() == sceneName;
		});

	if (it != instance.scenePtrs.end())
	{
		instance.pActiveScene = it->get();
		instance.sceneState = Scene::State::ssIDLE;
		return true;
	}

	return false;
}