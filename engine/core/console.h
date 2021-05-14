#pragma once
#include "common.h"

class Log
{
	friend class Console;

public:
	enum class Type
	{
		ltNOTIFICATION,
		ltWARNING,
		ltERROR
	};

public:
	Log(Type type, const std::string& message);

private:
	const Type type;
	std::string tag;
	const std::string message;
};

class Console
{
private:
	static Console singleton;

	std::vector<Log> logs;

public:
	static void AddLog(Log log);
	static void Clear();

	static void DrawWidget();
};