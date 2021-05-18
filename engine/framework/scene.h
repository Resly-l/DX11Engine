#pragma once
#include "system.h"

class Entity;
class SystemBase;

class Scene
{
public:
	enum class State
	{
		ssIDLE,
		ssPLAYING,
		ssPAUSED,
	};

private:
	// names are used as key value in scene manager
	// thus scene names must not be duplicated
	std::string name;

	// entity that has this name will become camera of the scene
	// if entity doens't have a camera component, scene assigns one
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
	void SetCameraName(const std::string& name) { cameraName = name; }

	Entity* GetRootEntity() const { return pRootEntity.get(); }
	Entity* GetSelectedEntity() const { return pSelectedEntity; }
	Entity* FindEntity(const std::string& entityName) const;
	void SelectEntity(Entity* pEntity);

	JSON ToJson() const;
	void FromJson(const JSON& json);

private:
	Entity* FindEntity(const std::unique_ptr<Entity>& pEntity, const std::string& entityName) const;
};