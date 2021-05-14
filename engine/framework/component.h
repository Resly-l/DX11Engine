#pragma once
#include "common.h"

typedef size_t ComponentID;

class ComponentBase
{
	friend class Entity;

	template<typename T>
	friend class Component;

protected:
	class Entity* pOwner = nullptr;

private:
	inline static ComponentID GID = 0;

public:
	virtual ~ComponentBase() = default;

public:
	virtual ComponentID GetID() const abstract;
	virtual std::string GetStringID() const abstract;

	virtual JSON ToJson() const abstract;
	virtual void FromJson(const JSON& json) abstract;

	virtual void DrawWidget() {};
};

template<typename T>
class Component : public ComponentBase
{
public:
	inline static const ComponentID ID = GID++;
	static const std::string stringID;

public:
	ComponentID GetID() const override { return ID; };
	std::string GetStringID() const override { return stringID; };
};

template<typename T>
const std::string Component<T>::stringID = typeid(T).name();