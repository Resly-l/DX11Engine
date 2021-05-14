#pragma once
#include "system.h"

class Entity;
class SystemBase;

class Scene
{
private:
	std::string name;
	std::string cameraName = "camera";

	std::unique_ptr<Entity> pRootEntity;
	Entity* pSelectedEntity = nullptr;

public:
	Scene();
	Scene(const std::string& name);
	~Scene();

public:
	std::string GetName() const { return name; }
	std::string GetCameraName() const { return cameraName; }

	void SetName(const std::string& name) { this->name = name; }

	// entity that has this name will become camera of the scene
	// if entity doens't have a camera component, scene assigns one
	void SetCameraName(const std::string& name) { cameraName = name; }

	Entity* GetRootEntity() const { return pRootEntity.get(); }
	Entity* GetSelectedEntity() const { return pSelectedEntity; }

	Entity* FindEntity(const std::string& entityName) const;
	void SelectEntity(Entity* pEntity);

	void Play(double deltaSeconds);

	JSON ToJson() const;
	void FromJson(const JSON& json);

private:
	Entity* FindEntity(const std::unique_ptr<Entity>& pEntity, const std::string& entityName) const;
};

class SceneManager
{
private:
	static SceneManager singleton;

	std::unordered_map<std::string, std::unique_ptr<Scene>> scenePtrs;
	Scene* pActiveScene = nullptr;

private:
	SceneManager() = default;

public:
	static bool SetActiveScene(const std::string& sceneName);
	static Scene* GetActiveScene() { return singleton.pActiveScene; }

	static void AddScene(std::unique_ptr<Scene> pScene);

	static bool SaveSceneToFile(const std::string& sceneName, const std::string& filePath);
	static bool LoadSceneFromFile(const std::string& filePath);

	static bool SwapScene(const std::string& sceneName);
	static void SwapSceneFromFile(const std::string& filePath);
};