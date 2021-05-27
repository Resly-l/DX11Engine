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

private:
	const Type type;
	const std::string message;
	std::string tag;

public:
	Log(Type type, std::string message);
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