#include "timer.h"

Timer::Timer()
{
	previous = std::chrono::high_resolution_clock::now();
}

double Timer::Peek() const
{
	return duration_cast<std::chrono::duration<double, std::ratio<1, 1>>>(std::chrono::high_resolution_clock::now() - previous).count();
}

double Timer::Mark()
{
	const auto now = std::chrono::high_resolution_clock::now();
	double delta = duration_cast<std::chrono::duration<double, std::ratio<1, 1>>>(now - previous).count();
	previous = now;
	return delta;
}