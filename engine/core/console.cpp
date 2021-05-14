#include "console.h"

Log::Log(Type type, const std::string& message)
	:
	type(type),
	message(message)
{
	switch (type)
	{
	case Type::ltNOTIFICATION:
		tag = "notification";
		break;
	case Type::ltWARNING:
		tag = "warning";
		break;
	case Type::ltERROR:
		tag = "error";
		break;
	}
}

Console Console::singleton;

void Console::AddLog(Log log)
{
	singleton.logs.push_back(std::move(log));
}

void Console::Clear()
{
	singleton.logs.clear();
}

void Console::DrawWidget()
{
	ImGui::BeginChildFrame(ImGui::GetID("console"),ImGui::GetContentRegionAvail());
	{
		auto MapLogColor = [](Log::Type type)
		{
			switch (type)
			{
			case Log::Type::ltWARNING:
				return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
			case Log::Type::ltERROR:
				return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
			default:
				return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
			}
		};

		for (auto& log : singleton.logs)
		{
			ImGui::AlignTextToFramePadding();
			ImGui::TextColored(MapLogColor(log.type), ("[" + log.tag + "] ").c_str());
			ImGui::SameLine();
			ImGui::Text(log.message.c_str());
		}
	}
	ImGui::EndChildFrame();
}