#pragma once
#include "scene.h"

class SceneManager
{
private:
	static SceneManager singleton;

	inline static const std::string temporarySavePath = "../asset/scenes/temp.scene";

	std::unordered_map<std::string, std::unique_ptr<Scene>> scenePtrs;

	Scene* pActiveScene = nullptr;
	Scene::State sceneState = Scene::State::ssIDLE;

	// overall scene play speed factor
	// certain systems are disabled if delta time is 0.0f
	float fDeltatimeFactor = 1.0f;

private:
	~SceneManager() = default;

public:
	static void Play(double deltaSeconds);

	static void SetDeltatimeFactor(float fFactor) { singleton.fDeltatimeFactor = fFactor; }
	static float GetDeltatimeFactor() { return singleton.fDeltatimeFactor; }

	static void SetSceneState(Scene::State state);
	static Scene::State GetSceneState() { return singleton.sceneState; }

	static bool SetActiveScene(const std::string& sceneName);
	static Scene* GetActiveScene() { return singleton.pActiveScene; }

	static void AddScene(std::unique_ptr<Scene> pScene);
	static bool RemoveScene(const std::string& sceneName);
	static bool RemoveScene(Scene* pTarget);

	static bool SaveSceneToFile(const std::string& sceneName, const std::string& filePath);
	static bool LoadSceneFromFile(const std::string& filePath);

	static bool SwapScene(const std::string& sceneName);
};