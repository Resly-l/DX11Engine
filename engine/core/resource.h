#pragma once
#include "common.h"

class Resource
{
public:
	virtual ~Resource() = default;
};

class ResourceCodex
{
public:
	template<typename ResourceType>
	static std::shared_ptr<ResourceType> Find(const std::string& key);

	// tries to find resource with given key
	// if not found, makes new one
	template<typename ResourceType, typename... Params>
	static std::shared_ptr<ResourceType> Resolve(const std::string& key, Params&&... params);

	static void ReleaseUnusedResources()
	{
		std::erase_if(resourcePtrs,
			[](const std::pair<std::string, std::shared_ptr<Resource>>& pair)
			{
				// if only place this resource gets used is here
				return pair.second.use_count() == 1;
			});
	}

private:
	inline static std::unordered_map<std::string, std::shared_ptr<Resource>> resourcePtrs;
};



template<typename ResourceType>
std::shared_ptr<ResourceType> ResourceCodex::Find(const std::string& key)
{
	if (auto it = resourcePtrs.find(key); it != resourcePtrs.end())
	{
		return std::static_pointer_cast<ResourceType>(it->second);
	}

	return nullptr;
}

template<typename ResourceType, typename... Params>
std::shared_ptr<ResourceType> ResourceCodex::Resolve(const std::string& key, Params&&... params)
{
	if (auto it = resourcePtrs.find(key); it != resourcePtrs.end())
	{
		return std::static_pointer_cast<ResourceType>(it->second);
	}
	else
	{
		auto pResource = std::make_shared<ResourceType>(std::forward<Params>(params)...);

		resourcePtrs[key] = pResource;

		return std::static_pointer_cast<ResourceType>(pResource);
	}
}