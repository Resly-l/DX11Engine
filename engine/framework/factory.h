#pragma once
#include "common.h"

template<typename IDType, typename BaseType>
class Factory
{
public:
	template<typename T>
	static void Register();

	static const std::vector<std::string>& GetRegisteredStringIDs() { return registeredStringIDs; }

	static BaseType* Create(const IDType& typeID);
	static BaseType* Create(const std::string& stringID);

	static bool Remove(BaseType* pTarget);

	static std::vector<std::unique_ptr<BaseType>>& GetInstances(const IDType& typeID);

private:
	inline static std::unordered_map<IDType, std::vector<std::unique_ptr<BaseType>>> instancePtrs;
	inline static std::unordered_map<IDType, std::pair<std::string, std::function<std::unique_ptr<BaseType>()>>> creators;

	inline static std::vector<std::string> registeredStringIDs;
};



template<typename IDType, typename BaseType>
template<typename T>
void Factory<IDType, BaseType>::Register()
{
	if (creators.find(T::ID) != creators.end())
	{
		return;
	}

	creators[T::ID] = std::make_pair(T::stringID, std::make_unique<T>);
	registeredStringIDs.push_back(T::stringID);
}

template<typename IDType, typename BaseType>
BaseType* Factory<IDType, BaseType>::Create(const IDType& typeID)
{
	if (auto it = creators.find(typeID); it != creators.end())
	{
		instancePtrs[typeID].push_back(it->second.second());
		return instancePtrs[typeID].back().get();
	}

	return nullptr;
}

template<typename IDType, typename BaseType>
BaseType* Factory<IDType, BaseType>::Create(const std::string& stringID)
{
	auto it = std::find_if(creators.begin(), creators.end(),
		[&stringID](const std::pair<IDType, std::pair<std::string, std::function<std::unique_ptr<BaseType>()>>>& pair)
		{
			return pair.second.first == stringID;
		});

	if (it != creators.end())
	{
		instancePtrs[it->first].push_back(it->second.second());
		return instancePtrs[it->first].back().get();
	}

	return nullptr;
}

template<typename IDType, typename BaseType>
bool Factory<IDType, BaseType>::Remove(BaseType* pTarget)
{
	return std::erase_if(instancePtrs[pTarget->GetID()],
		[&pTarget](const std::unique_ptr<BaseType>& pInstance)
		{
			return pInstance.get() == pTarget;
		}) != 0;
}

template<typename IDType, typename BaseType>
std::vector<std::unique_ptr<BaseType>>& Factory<IDType, BaseType>::GetInstances(const IDType& typeID)
{
	return instancePtrs[typeID];
}