#include "event.h"

void EventManager::Emit(const Event& event)
{
	for (auto& [callback, pSubscribers] : subscribers[event.name])
	{
		callback(event);
	}
}

void EventManager::Subscribe(const std::string& eventName, std::function<void(const Event&)> function, void* pSubscriber)
{
	subscribers[eventName].push_back({ function, pSubscriber });
}

void EventManager::Unsubscribe(void* pSubscriber)
{
	for (auto& [eventName, subscribers] : subscribers)
	{
		std::erase_if(subscribers,
			[pSubscriber](const std::pair<std::function<void(const Event&)>, void*>& pair)
			{
				return pair.second == pSubscriber;
			});
	}
}

void EventManager::ClearSubscribers(const std::string& eventName)
{
	if (eventName.empty())
	{
		subscribers.clear();
	}
	else
	{
		subscribers[eventName].clear();
	}
}