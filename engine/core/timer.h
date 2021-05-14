#pragma once
#include "common.h"
#include <chrono>

// unit = millisecond
class Timer
{
private:
	std::chrono::high_resolution_clock::time_point previous;

public:
	Timer();
	virtual ~Timer() = default;

public:
	double Peek() const;
	double Mark();
};