#pragma once
#include "component_factory.h"
#include "node_graph.h"

class Entity
{
	friend class Scene;

private:
	std::string name;

	Entity* pParent = nullptr;
	std::vector<std::unique_ptr<Entity>> childPtrs;

	std::unordered_map<ComponentID, ComponentBase*> componentPtrs;
	NodeGraph script;

public:
	Entity();
	Entity(const std::string& name);
	~Entity();

public:
	void SetName(const std::string& name);
	const std::string& GetName() const;

	template<typename ComponentType>
	ComponentType* AssignComponent();
	ComponentBase* AssignComponent(ComponentID ID, const std::string& stringID);
	template<typename ComponentType>
	ComponentType* GetComponent();
	ComponentBase* GetComponent(ComponentID ID);
	template<typename ComponentType>
	bool RemoveComponent();
	bool RemoveComponent(ComponentID ID);

	void ReleaseComponents();

	void AddChild(std::unique_ptr<Entity> pChild);
	bool RemoveChild(Entity* pChild);

	Entity* GetParent() const { return pParent; }
	std::vector<std::unique_ptr<Entity>>& GetChildren();

	JSON ToJson() const;
	void FromJson(const JSON& json);
};



template<typename ComponentType>
ComponentType* Entity::AssignComponent()
{
	auto pComponent = ComponentFactory::Create(ComponentType::ID);
	pComponent->pOwner = this;
	componentPtrs[ComponentType::ID] = pComponent;
	return static_cast<ComponentType*>(pComponent);
}

template<typename ComponentType>
ComponentType* Entity::GetComponent()
{
	if (auto it = componentPtrs.find(ComponentType::ID); it != componentPtrs.end())
	{
		return static_cast<ComponentType*>(it->second);
	}

	return nullptr;
}

template<typename ComponentType>
bool Entity::RemoveComponent()
{
	if (auto it = componentPtrs.find(ComponentType::ID); it != componentPtrs.end())
	{
		return ComponentFactory::Remove(it.first, it.secomd);
		componentPtrs.erase(it);
	}

	return false;
}