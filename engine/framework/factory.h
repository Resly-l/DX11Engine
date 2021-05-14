#pragma once
#include "common.h"

template<typename IDTy, typename BaseTy>
class Factory
{
private:
	inline static std::unordered_map<IDTy, std::vector<std::unique_ptr<BaseTy>>> instancePtrs;
	inline static std::unordered_map<IDTy, std::pair<std::string, std::function<std::unique_ptr<BaseTy>()>>> creators;

public:
	 template<typename T>
	static void Register();

	static BaseTy* Create(const IDTy& typeID);
	static BaseTy* Create(const std::string& stringID);

	static bool Remove(BaseTy* pTarget);

	static std::vector<std::unique_ptr<BaseTy>>& GetInstances(const IDTy& typeID);
};



template<typename IDTy, typename BaseTy>
template<typename T>
void Factory<IDTy, BaseTy>::Register()
{
	creators[T::ID] = std::make_pair(T::stringID, std::make_unique<T>);
}

template<typename IDTy, typename BaseTy>
BaseTy* Factory<IDTy, BaseTy>::Create(const IDTy& typeID)
{
	if (auto it = creators.find(typeID); it != creators.end())
	{
		instancePtrs[typeID].push_back(it->second.second());
		return instancePtrs[typeID].back().get();
	}

	return nullptr;
}

template<typename IDTy, typename BaseTy>
BaseTy* Factory<IDTy, BaseTy>::Create(const std::string& stringID)
{
	auto it = std::find_if(creators.begin(), creators.end(),
		[&stringID](const std::pair<IDTy, std::pair<std::string, std::function<std::unique_ptr<BaseTy>()>>>& pair)
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

template<typename IDTy, typename BaseTy>
bool Factory<IDTy, BaseTy>::Remove(BaseTy* pTarget)
{
	return std::erase_if(instancePtrs[pTarget->GetID()],
		[&pTarget](const std::unique_ptr<BaseTy>& pInstance)
		{
			return pInstance.get() == pTarget;
		}) != 0;
}

template<typename IDTy, typename BaseTy>
std::vector<std::unique_ptr<BaseTy>>& Factory<IDTy, BaseTy>::GetInstances(const IDTy& typeID)
{
	return instancePtrs[typeID];
}