#pragma once
#include "common.h"

typedef size_t SystemID;

class SystemBase
{
	template<typename T>
	friend class System;

private:
	inline static SystemID GID = 0;

public:
	virtual ~SystemBase() = default;

public:
	virtual SystemID GetID() const abstract;
	virtual std::string GetStringID() const abstract;

	virtual void Update(double deltaSeconds) abstract;
	virtual void DrawWidget() {}
};

DECLARE_CRTP(System, SystemID)