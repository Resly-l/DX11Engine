#include "console.h"

Console Console::instance;

Log::Log(Type type, std::string message)
	:
	type(type),
	message(std::move(message))
{
	switch (type)
	{
	case Log::Type::ltNOTIFICATION:
		tag = "notification";
		break;
	case Log::Type::ltWARNING:
		tag = "warning";
		break;
	case Log::Type::ltERROR:
		tag = "error";
		break;
	}
}

void Console::AddLog(Log log)
{
	instance.logs.push_back(std::move(log));
}

void Console::Clear()
{
	instance.logs.clear();
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

		for (auto& log : instance.logs)
		{
			ImGui::AlignTextToFramePadding();
			ImGui::TextColored(MapLogColor(log.type), ("[" + log.tag + "] ").c_str());
			ImGui::SameLine();
			ImGui::Text(log.message.c_str());
		}
	}
	ImGui::EndChildFrame();
}