#include "system.h"

SystemContainer SystemContainer::singleton;

std::vector<std::unique_ptr<SystemBase>>& SystemContainer::GetSystems()
{
	return singleton.systemPtrs;
}