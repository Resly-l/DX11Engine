#include "scene_controller.h"
#include "scene_manager.h"
#include "systems/render_system.h"

SceneController::SceneController()
	:
	playIcon(Texture::Type::ttUI, "../asset/images/icons/play.png"),
	pauseIcon(Texture::Type::ttUI, "../asset/images/icons/pause.png"),
	stopIcon(Texture::Type::ttUI, "../asset/images/icons/stop.png")
{}

void SceneController::Draw()
{
	ImGui::Begin("Scene Player", nullptr);
	{
		static const float fButtonSize = 50.0f;

		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - (fButtonSize * 3.0f + 14)) * 0.5f);
		ImGui::SetCursorPosY((ImGui::GetWindowHeight() - fButtonSize) * 0.5f);

		if (ImGui::ImageButton(playIcon.GetShaderResourceView(), { fButtonSize, fButtonSize }, { 0 ,0 }, { 1, 1 }, 0))
		{
			SceneManager::SetSceneState(Scene::State::ssPLAYING);
		}

		ImGui::SameLine();
		if (ImGui::ImageButton(pauseIcon.GetShaderResourceView(), { fButtonSize, fButtonSize }, { 0, 0 }, { 1, 1 }, 0))
		{
			SceneManager::SetSceneState(Scene::State::ssPAUSED);
		}

		ImGui::SameLine();
		if (ImGui::ImageButton(stopIcon.GetShaderResourceView(), { fButtonSize, fButtonSize }, { 0, 0 }, { 1, 1 }, 0))
		{
			SceneManager::SetSceneState(Scene::State::ssIDLE);
		}
	}
	ImGui::End();
}