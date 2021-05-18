#include "file_manager.h"
#include "console.h"

std::string FileManager::SelectFileToLoad(const std::string& initialPath, const char* extentions)
{
	std::string filePath;

	OPENFILENAMEA ofn;
	char szFile[260] = { 0 };

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = extentions;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = initialPath.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameA(&ofn) == TRUE)
	{
		filePath = std::filesystem::relative(ofn.lpstrFile).string();
	}

	return filePath;
}

std::string FileManager::SelectFileToSave(const std::string& initialPath)
{
	std::string filePath;

	OPENFILENAMEA ofn;
	char szFile[260] = { 0 };

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "all(*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = initialPath.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetSaveFileNameA(&ofn) == TRUE)
	{
		filePath = std::filesystem::relative(ofn.lpstrFile).string();
	}

	return filePath;
}

JSON FileManager::ReadFile(const std::string& filePath)
{
	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	std::string buffer;
	JSON json;

	try
	{
		file.open(filePath, std::ifstream::binary | std::ifstream::ate);
		auto size = file.tellg();

		buffer.resize(size);
		file.seekg(0, std::ios_base::beg);
		file.read(buffer.data(), size);

		json = JSON::parse(buffer);
	}
	catch (const std::exception& e)
	{
		Console::AddLog({ Log::Type::ltERROR, e.what() });
	}

	file.close();

	return json;
}

bool FileManager::CreateFileToPath(const std::string& filePath, const JSON& initialData)
{
	std::ofstream file;
	std::string dump = initialData.dump(4);
	bool bException = false;

	try
	{
		file.open(filePath, std::ios::binary | std::ios::trunc);
		file.write(dump.data(), dump.size());
	}
	catch (const std::exception& e)
	{
		Console::AddLog({ Log::Type::ltERROR, ("failed to create file : " + filePath + e.what()) });
		bException = true;
	}

	file.close();

	return !bException;
}

bool FileManager::DeleteFileFromPath(const std::string& filePath)
{
	if (remove(filePath.c_str()) < 0)
	{
		Console::AddLog({ Log::Type::ltWARNING, "file tried to delete doesn't exist" });
		return false;
	}
	return true;
}