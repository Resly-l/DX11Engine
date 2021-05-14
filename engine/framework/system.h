#pragma once
#include "common.h"

typedef size_t SystemID;

class SystemBase
{
	template<typename T>
	friend class System;

private:
	inline static SystemID GID = 0;

	bool bActive = true;

public:
	virtual ~SystemBase() = default;

public:
	virtual SystemID GetID() const abstract;

	bool IsActive() const { return bActive; }
	void SetActiveness(bool bActive) { this->bActive = bActive; };

	virtual void Update(double deltaSeconds) abstract;
	virtual void DrawWidget() {}
};

template<typename T>
class System : public SystemBase
{
public:
	inline static const SystemID ID = GID++;

public:
	SystemID GetID() const override { return ID; }
};

// global systems container
class SystemContainer
{
private:
	static SystemContainer singleton;

	// stores system pointers in two containers
	// one for traversing, another for searching
	std::vector<std::unique_ptr<SystemBase>> systemPtrs;
	std::unordered_map<SystemID, SystemBase*> systemCache;

public:
	template<typename SystemType>
	static SystemType* AddSystem();
	template<typename SystemType>
	static SystemType* GetSystem();

	static std::vector<std::unique_ptr<SystemBase>>& GetSystems();
};



template<typename SystemType>
static SystemType* SystemContainer::AddSystem()
{
	if (auto it = singleton.systemCache.find(SystemType::ID); it == singleton.systemCache.end())
	{
		singleton.systemPtrs.push_back(std::make_unique<SystemType>());
		singleton.systemCache[SystemType::ID] = singleton.systemPtrs.back().get();

		return static_cast<SystemType*>(singleton.systemPtrs.back().get());
	}
	else
	{
		return static_cast<SystemType*>(it->second);
	}
}

template<typename SystemType>
static SystemType* SystemContainer::GetSystem()
{
	if (auto it = singleton.systemCache.find(SystemType::ID); it != singleton.systemCache.end())
	{
		return static_cast<SystemType*>(it->second);
	}

	return nullptr;
}