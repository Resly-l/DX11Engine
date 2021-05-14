#include "entity.h"

// global entity count
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

ComponentBase* Entity::AssignComponent(ComponentID ID, const std::string& stringID)
{
	if (auto it = componentPtrs.find(ID); it != componentPtrs.end())
	{
		return it->second;
	}

	if (auto pComponent = ComponentFactory::Create(stringID))
	{
		pComponent->pOwner = this;
		componentPtrs[ID] = pComponent;

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

void Entity::ReleaseComponents()
{
	for (auto& [typeID, pComponent] : componentPtrs)
	{
		ComponentFactory::Remove(pComponent);
	}
}

void Entity::AddChild(std::unique_ptr<Entity> pChild)
{
	pChild->pParent = this;
	childPtrs.push_back(std::move(pChild));
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

	return json;
}

void Entity::FromJson(const JSON& json)
{
	ReleaseComponents();

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

			componentPtrs[pComponent->GetID()] = pComponent;
		}
	}
}