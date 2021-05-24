#include "event.h"

void EventManager::Emit(const Event& event)
{
	for (auto& [callback, pSubscribers] : subscribers[event.name])
	{
		callback(event);
	}
}

void EventManager::Subscribe(const std::string& eventName, std::function<void(const Event&)> function, const std::string& subscriptionKey)
{
	subscribers[eventName].push_back({ function, subscriptionKey });
}

void EventManager::Unsubscribe(const std::string& subscriptionKey)
{
	for (auto& [eventName, subscribers] : subscribers)
	{
		std::erase_if(subscribers,
			[&subscriptionKey](const std::pair<std::function<void(const Event&)>, std::string>& pair)
			{
				return pair.second == subscriptionKey;
			});
	}
}

void EventManager::Clear()
{
	subscribers.clear();
}