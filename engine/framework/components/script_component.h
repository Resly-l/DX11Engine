#pragma once
#include "component.h"
#include "node_graph.h"

class ScriptComponent : public Component<ScriptComponent>
{
private:
	NodeGraph nodeGraph;

public:
	void Execute(double deltaSeconds);

	bool LoadFromFile(const std::string& filePath);
	bool SaveToFile(const std::string& filePath);

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;
	void DrawEditor();
};