#include "functions.h"

std::wstring ToWideString(const std::string& string)
{
	wchar_t wide[512] = {};
	mbstowcs_s(nullptr, wide, string.c_str(), _TRUNCATE);
	return std::wstring(wide);
}

std::string ToNarrowString(const std::wstring& wstring)
{
	char narrow[512] = {};
	wcstombs_s(nullptr, narrow, wstring.c_str(), _TRUNCATE);
	return std::string(narrow);
}