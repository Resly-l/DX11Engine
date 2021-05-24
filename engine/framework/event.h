#pragma once
#include "common.h"

struct Event
{
	std::string name;
	JSON data;
};

class EventManager
{
public:
	static void Emit(const Event& event);

	static void Subscribe(const std::string& eventName, std::function<void(const Event&)> function, const std::string& subscriptionKey);
	static void Unsubscribe(const std::string& subscriptionKey);

	static void Clear();

private:
	// unordered_map<eventName, vector<callback, subscriptionKey>>
	inline static std::unordered_map<std::string, std::vector<std::pair<std::function<void(const Event&)>, std::string>>> subscribers;
};