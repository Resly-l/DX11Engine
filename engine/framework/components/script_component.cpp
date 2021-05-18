#include "script_component.h"
#include "node_factory.h"
#include "file_manager.h"
#include "input/keyboard.h"
#include "input/mouse.h"

void ScriptComponent::Execute(double deltaSeconds)
{
	nodeGraph.SyncNodes(deltaSeconds);
}

bool ScriptComponent::LoadFromFile(const std::string& filePath)
{
	if (auto scriptJson = FileManager::ReadFile(filePath); scriptJson.is_null() == false)
	{
		nodeGraph.FromJson(scriptJson);
		return true;
	}

	return false;
}

bool ScriptComponent::SaveToFile(const std::string& filePath)
{
	return FileManager::CreateFileToPath(filePath, nodeGraph.ToJson());
}

JSON ScriptComponent::ToJson() const
{
	JSON json;

	json["node_graph"] = nodeGraph.ToJson();

	return json;
}

void ScriptComponent::FromJson(const JSON& json)
{
	nodeGraph.FromJson(json["node_graph"]);
}

void ScriptComponent::DrawWidget()
{
	if (ImGui::Button("Load From File"))
	{
		auto scriptPath = FileManager::SelectFileToLoad("../asset/scripts/", "Script(*.script)\0*.script\0");
		if (scriptPath.empty() == false)
		{
			LoadFromFile(scriptPath);
		}
	}

	if (ImGui::Button("Save To File"))
	{
		auto savePath = FileManager::SelectFileToSave("../asset/scripts/");
		if (savePath.empty() == false)
		{
			SaveToFile(savePath);
		}
	}
}

void ScriptComponent::DrawEditor()
{
	nodeGraph.DrawEditor();
}