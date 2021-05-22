#include "keyboard.h"
#include "imgui/imgui_impl_win32.h"

Keyboard Keyboard::instance;

bool Keyboard::IsKeyFree(unsigned char key)
{
	return instance.currentKeyStates[key] == KeyState::ksFREE;
}

bool Keyboard::IsKeyDown(unsigned char key)
{
	return instance.currentKeyStates[key] == KeyState::ksDOWN;
}

bool Keyboard::IsKeyHold(unsigned char key)
{
	return instance.currentKeyStates[key] == KeyState::ksHOLD;
}

bool Keyboard::IsKeyUp(unsigned char key)
{
	return instance.currentKeyStates[key] == KeyState::ksUP;
}

bool Keyboard::IsAutoRepeatEnabled()
{
	return instance.enableAutoRepeat;
}

void Keyboard::SetAutoRepeat(bool bAutoRepeat)
{
	bAutoRepeat = bAutoRepeat;
}

bool Keyboard::HandleWM(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		if (!ImGui::GetIO().WantCaptureKeyboard && (Keyboard::IsAutoRepeatEnabled() || !(lParam & 0x40000000)))
		{
			instance.currentKeyStates[(unsigned char)wParam] = KeyState::ksDOWN;
		}
		break;
	case WM_SYSKEYUP:
	case WM_KEYUP:
		instance.currentKeyStates[(unsigned char)wParam] = KeyState::ksUP;
		break;
	default:
		return false;
	}

	return true;
}

void Keyboard::Update()
{
	for (int i = 0; i < numKeys; i++)
	{
		if (instance.previousKeyStates[i] == KeyState::ksDOWN && instance.currentKeyStates[i] == KeyState::ksDOWN)
		{
			instance.currentKeyStates[i] = KeyState::ksHOLD;
		}
		else if (instance.previousKeyStates[i] == KeyState::ksUP && instance.currentKeyStates[i] == KeyState::ksUP)
		{
			instance.currentKeyStates[i] = KeyState::ksFREE;
		}
		instance.previousKeyStates[i] = instance.currentKeyStates[i];
	}
}