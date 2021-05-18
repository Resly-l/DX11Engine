#pragma once
#include "system.h"

// global systems container
class SystemContainer
{
private:
	// storing same system pointers in two containers
	// one for traversing, another for searching
	inline static std::vector<std::unique_ptr<SystemBase>> systemPtrs;
	inline static std::unordered_map<SystemID, SystemBase*> systemCache;

public:
	template<typename SystemType>
	static SystemType* AddSystem();
	template<typename SystemType>
	static bool RemoveSystem();
	template<typename SystemType>
	static SystemType* GetSystem();

	static std::vector<std::unique_ptr<SystemBase>>& GetSystems() { return systemPtrs; };
};



template<typename SystemType>
static SystemType* SystemContainer::AddSystem()
{
	if (auto it = systemCache.find(SystemType::ID); it == systemCache.end())
	{
		systemPtrs.push_back(std::make_unique<SystemType>());
		systemCache[SystemType::ID] = systemPtrs.back().get();

		return static_cast<SystemType*>(systemPtrs.back().get());
	}
	else
	{
		return static_cast<SystemType*>(it->second);
	}
}

template<typename SystemType>
bool SystemContainer::RemoveSystem()
{
	if (auto it = systemCache.find(SystemType::ID); it != systemCache.end())
	{
		std::erase_if(systemPtrs,
			[&it](const std::unique_ptr<SystemBase>& pSystem)
			{
				return pSystem.get() == it->second;
			});

		systemCache.erase(it);

		return true;
	}

	return false;
}

template<typename SystemType>
static SystemType* SystemContainer::GetSystem()
{
	if (auto it = systemCache.find(SystemType::ID); it != systemCache.end())
	{
		return static_cast<SystemType*>(it->second);
	}

	return nullptr;
}