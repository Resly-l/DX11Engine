#pragma once
#include "resource.h"

class FileManager
{
public:
	// second parameter contains null characters which can't be assigned to std::string
	static std::string SelectFileToLoad(const std::string& initialPath, const char* extentions);
	static std::string SelectFileToSave(const std::string& initialPath);

	static JSON ReadFile(const std::string& filePath);

	static bool CreateFileToPath(const std::string& filePath, const JSON& initialData);
	static bool DeleteFileFromPath(const std::string& filePath);
};