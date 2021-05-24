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
	Log(Type type, std::string message);

private:
	const Type type;
	const std::string message;
	std::string tag;
};

class Console
{
public:
	static void AddLog(Log log);
	static void Clear();

	static void DrawWidget();

private:
	static Console instance;

	std::vector<Log> logs;
};