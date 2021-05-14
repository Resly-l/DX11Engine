#pragma once
#include "common.h"

struct Event
{
	std::string name;
	JSON data;
};

class EventManager
{
private:
	inline static std::unordered_map<std::string, std::vector<std::pair<std::function<void(const Event&)>, void*>>> subscribers;

public:
	static void Emit(const Event& event);

	static void Subscribe(const std::string& eventName, std::function<void(const Event&)> function, void* pSubscriber = nullptr);
	static void Unsubscribe(void* pSubscriber);

	static void ClearSubscribers(const std::string& eventName = "");
};