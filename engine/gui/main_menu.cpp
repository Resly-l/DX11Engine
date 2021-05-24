#include "main_menu.h"
#include "scene_manager.h"
#include "file_manager.h"

void MainMenu::Draw()
{
	ImGui::BeginMainMenuBar();
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene"))
			{
				CreateNewScene();
				savedScenePath.clear();
			}
			if (ImGui::MenuItem("Open Scene"))
			{
				OpenSceneFromFile();
			}
			if (ImGui::MenuItem("Save Scene"))
			{
				SaveSceneToFile(false);
			}
			if (ImGui::MenuItem("Save Scene As"))
			{
				SaveSceneToFile(true);
			}

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void MainMenu::CreateNewScene()
{
	SceneManager::ClearScenes();

	auto pScene = SceneManager::AddScene(std::make_unique<Scene>());
	SceneManager::SetActiveScene(pScene);
}

void MainMenu::OpenSceneFromFile()
{
	const auto filePath = FileManager::SelectFileToLoad("../asset/scenes/", "scene(*.scene)\0*.scene\0");

	if (filePath.empty())
	{
		return;
	}

	SceneManager::ClearScenes();

	const auto sceneJson = FileManager::ReadFile(filePath);
	auto pScene = SceneManager::AddScene(std::make_unique<Scene>());

	pScene->FromJson(sceneJson);
	SceneManager::SetActiveScene(pScene);

	savedScenePath = filePath;
}

void MainMenu::SaveSceneToFile(bool bSelectPath)
{
	auto pScene = SceneManager::GetActiveScene();

	if (pScene == nullptr)
	{
		return;
	}

	if (bSelectPath == false && savedScenePath.empty() == false)
	{
		FileManager::CreateFileToPath(savedScenePath, pScene->ToJson());
	}
	else
	{
		auto filePath = FileManager::SelectFileToSave("../asset/scenes/");

		if (filePath.empty())
		{
			return;
		}

		FileManager::CreateFileToPath(filePath, pScene->ToJson());

		savedScenePath = filePath;
	}
}