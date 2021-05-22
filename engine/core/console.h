#pragma once
#include "common.h"

struct Log
{
	enum class Type
	{
		ltNOTIFICATION,
		ltWARNING,
		ltERROR
	};

	const Type type;
	const std::string message;
	std::string tag;
};

class Console
{
private:
	static Console instance;

	std::vector<Log> logs;

public:
	static void AddLog(Log log);
	static void Clear();

	static void DrawWidget();
};