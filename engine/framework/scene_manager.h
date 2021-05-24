#pragma once
#include "scene.h"

class SceneManager
{
public:
	static void Play(double deltaSeconds);

	static void SetSceneState(Scene::State state);
	static Scene::State GetSceneState() { return instance.sceneState; }

	static bool SetActiveScene(const std::string& sceneName);
	static bool SetActiveScene(Scene* pScene);
	static Scene* GetActiveScene() { return instance.pActiveScene; }

	static Scene* AddScene(std::unique_ptr<Scene> pScene);
	static bool RemoveScene(const std::string& sceneName);
	static bool RemoveScene(Scene* pTarget);
	static void ClearScenes();

	static bool SaveSceneToFile(const std::string& sceneName, const std::string& filePath);
	static bool LoadSceneFromFile(const std::string& filePath);

	static bool SwapScene(const std::string& sceneName);

private:
	static SceneManager instance;

	inline static const std::string temporarySavePath = "../asset/scenes/temp.scene";

	std::vector<std::unique_ptr<Scene>> scenePtrs;

	Scene* pActiveScene = nullptr;
	Scene::State sceneState = Scene::State::ssIDLE;
};