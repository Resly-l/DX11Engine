#pragma once
#include "common.h"

class Entity;

typedef size_t ComponentID;

class ComponentBase
{
	friend class Entity;

	// friend for crtp id
	template<typename T>
	friend class Component;

private:
	inline static ComponentID GID = 0;

	class Entity* pOwner = nullptr;

public:
	virtual ~ComponentBase() = default;

public:
	virtual ComponentID GetID() const = 0;
	virtual std::string GetStringID() const = 0;

	Entity* GetOwner() const { return pOwner; }

	virtual JSON ToJson() const = 0;
	virtual void FromJson(const JSON& json) = 0;

	virtual void DrawWidget() {};
};

DECLARE_CRTP(Component, ComponentID)