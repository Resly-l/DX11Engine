#include "engine_application.h"
#include "rendering/renderer.h"

#include "entity.h"
#include "node_factory.h"
#include "scene_manager.h"
#include "system_container.h"

#include "systems/render_system.h"
#include "systems/script_system.h"
#include "rendering/texture.h"

#include "gui/workspace.h"
#include "gui/entity_inspector.h"
#include "gui/scene_viewer.h"
#include "gui/scene_controller.h"
#include "gui/system_viewer.h"
#include "gui/toolbar.h"

EngineApplication::EngineApplication()
	:
	Window("Twilight Engine", "icon.ico", 1800, 900)
{
	ComponentFactory::Initialize();
	NodeFactory::Initialize();

	auto pRenderSystem = SystemContainer::AddSystem<RenderSystem>();
	auto pScriptSystem = SystemContainer::AddSystem<ScriptSystem>();

	pRenderSystem->SetEditorCameraUsage(true);
	pRenderSystem->SetSkyboxTexture(std::make_shared<TextureCube>("../asset/images/ocean/"));

	SceneManager::AddScene(std::make_unique<Scene>("unnamed_scene"));
	SceneManager::SetActiveScene("unnamed_scene");

	engineGUIs.push_back(std::make_unique<Workspace>());
	engineGUIs.push_back(std::make_unique<EntityInspector>());
	engineGUIs.push_back(std::make_unique<SceneViewer>());
	engineGUIs.push_back(std::make_unique<SceneController>());
	engineGUIs.push_back(std::make_unique<SystemViewer>());
	engineGUIs.push_back(std::make_unique<Toolbar>());
}

void EngineApplication::Run()
{
	while (Window::ProcessMessage())
	{
		double deltaSeconds = frameTimer.Mark();

		Renderer::BeginFrame();

		SceneManager::Play(deltaSeconds);

		BindBackbuffer();
		DrawGUI();

		Renderer::EndFrame();
	}
}

void EngineApplication::DrawGUI()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("background", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus);
	{
		ImGui::PopStyleVar();

		ImGuiID dockspaceId = ImGui::GetID("gui_dockspace");
		ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
	}
	ImGui::End();

	for (auto& pGUI : engineGUIs)
	{
		pGUI->Draw();
	}
}

void EngineApplication::BindBackbuffer()
{
	auto pBackbuffer = Renderer::GetBackbuffer();
	Renderer::GetContext()->OMSetRenderTargets(1, &pBackbuffer, nullptr);
}