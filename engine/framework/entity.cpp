#include "entity.h"

static size_t uEntityCount = 0;

Entity::Entity()
	:
	name("unnamed_entity_#" + std::to_string(uEntityCount++))
{}

Entity::Entity(const std::string& name)
	:
	name(name)
{}

Entity::~Entity()
{
	ReleaseComponents();
}

void Entity::SetName(const std::string& name)
{
	this->name = name;
}

const std::string& Entity::GetName() const
{
	return name;
}

ComponentBase* Entity::AssignComponent(ComponentID ID)
{
	if (auto pComponent = ComponentFactory::Create(ID))
	{
		pComponent->pOwner = this;
		componentPtrs[ID] = pComponent;
		return pComponent;
	}

	return nullptr;
}

ComponentBase* Entity::AssignComponent(const std::string& stringID)
{
	auto it = std::find_if(componentPtrs.begin(), componentPtrs.end(),
		[&stringID](const std::pair<ComponentID, ComponentBase*>& pair)
		{
			return pair.second->GetStringID() == stringID;
		});

	if (it != componentPtrs.end())
	{
		return it->second;
	}

	if (auto pComponent = ComponentFactory::Create(stringID))
	{
		pComponent->pOwner = this;
		componentPtrs[pComponent->GetID()] = pComponent;

		return pComponent;
	}

	return nullptr;
}

ComponentBase* Entity::GetComponent(ComponentID ID)
{
	if (auto it = componentPtrs.find(ID); it != componentPtrs.end())
	{
		return it->second;
	}

	return nullptr;
}

ComponentBase* Entity::GetComponent(const std::string& stringID)
{
	auto it = std::find_if(componentPtrs.begin(), componentPtrs.end(),
		[&stringID](const std::pair<ComponentID, ComponentBase*>& pair)
		{
			return pair.second->GetStringID() == stringID;
		});

	if (it != componentPtrs.end())
	{
		return it->second;
	}

	return nullptr;
}

bool Entity::RemoveComponent(ComponentID ID)
{
	if (auto it = componentPtrs.find(ID); it != componentPtrs.end())
	{
		ComponentFactory::Remove(it->second);
		componentPtrs.erase(it);

		return true;
	}

	return false;
}

bool Entity::RemoveComponent(const std::string& stringID)
{
	auto it = std::find_if(componentPtrs.begin(), componentPtrs.end(),
		[&stringID](const std::pair<ComponentID, ComponentBase*>& pair)
		{
			return pair.second->GetStringID() == stringID;
		});

	if (it != componentPtrs.end())
	{
		ComponentFactory::Remove(it->second);
		componentPtrs.erase(it);
		return true;
	}

	return false;
}

void Entity::ReleaseComponents()
{
	for (auto& [typeID, pComponent] : componentPtrs)
	{
		ComponentFactory::Remove(pComponent);
	}
}

Entity* Entity::AddChild(std::unique_ptr<Entity> pChild)
{
	pChild->pParent = this;
	childPtrs.push_back(std::move(pChild));
	return childPtrs.back().get();
}

bool Entity::RemoveChild(Entity* pTargetChild)
{
	return std::erase_if(childPtrs,
		[pTargetChild](const std::unique_ptr<Entity>& pChild)
		{
			return pChild.get() == pTargetChild;
		}) != 0;
}

std::vector<std::unique_ptr<Entity>>& Entity::GetChildren()
{
	return childPtrs;
}

JSON Entity::ToJson() const
{
	JSON json;

	json["name"] = name;

	for (auto& [typeID, pComponent] : componentPtrs)
	{
		auto componentJson = pComponent->ToJson();
		componentJson["string_id"] = pComponent->GetStringID();

		json["components"].push_back(componentJson);
	}

	for (auto& pChild : childPtrs)
	{
		json["children"].push_back(pChild->ToJson());
	}

	return json;
}

void Entity::FromJson(const JSON& json)
{
	ReleaseComponents();
	childPtrs.clear();

	if (json.contains("name"))
	{
		name = json["name"];
	}

	if (json.contains("components"))
	{
		for (auto& componentJson : json["components"])
		{
			std::string stringID = componentJson["string_id"];
			auto pComponent = ComponentFactory::Create(stringID);
			pComponent->FromJson(componentJson);
			pComponent->pOwner = this;

			componentPtrs[pComponent->GetID()] = pComponent;
		}
	}

	if (json.contains("children"))
	{
		for (auto& childJson : json["children"])
		{
			auto pChild = AddChild(std::make_unique<Entity>());
			pChild->FromJson(childJson);
		}
	}
}