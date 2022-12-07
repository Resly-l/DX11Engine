#pragma once
#include <string>

std::wstring ToWideString (const std::string& string);
std::string ToNarrowString (const std::wstring& wstring);

template<typename T>
T Squared(const T& val)
{
	return val * val;
}