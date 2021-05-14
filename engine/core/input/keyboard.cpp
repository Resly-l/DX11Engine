#include "keyboard.h"
#include "imgui/imgui_impl_win32.h"

Keyboard Keyboard::singleton;

bool Keyboard::IsKeyFree(unsigned char key)
{
	return singleton.currentKeyStates[key] == KeyState::ksFREE;
}

bool Keyboard::IsKeyDown(unsigned char key)
{
	return singleton.currentKeyStates[key] == KeyState::ksDOWN;
}

bool Keyboard::IsKeyHold(unsigned char key)
{
	return singleton.currentKeyStates[key] == KeyState::ksHOLD;
}

bool Keyboard::IsKeyUp(unsigned char key)
{
	return singleton.currentKeyStates[key] == KeyState::ksUP;
}

bool Keyboard::IsAutoRepeatEnabled()
{
	return singleton.bAutoRepeat;
}

void Keyboard::SetAutoRepeat(bool bAutoRepeat)
{
	bAutoRepeat = bAutoRepeat;
}

bool Keyboard::HandleWM(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		if (!ImGui::GetIO().WantCaptureKeyboard && (Keyboard::IsAutoRepeatEnabled() || !(lParam & 0x40000000)))
		{
			singleton.currentKeyStates[(unsigned char)wParam] = KeyState::ksDOWN;
		}
		break;
	case WM_SYSKEYUP:
	case WM_KEYUP:
		singleton.currentKeyStates[(unsigned char)wParam] = KeyState::ksUP;
		break;
	default:
		return false;
	}

	return true;
}

void Keyboard::Update()
{
	for (int i = 0; i < iNumKeys; i++)
	{
		if (singleton.previousKeyStates[i] == KeyState::ksDOWN && singleton.currentKeyStates[i] == KeyState::ksDOWN)
		{
			singleton.currentKeyStates[i] = KeyState::ksHOLD;
		}
		else if (singleton.previousKeyStates[i] == KeyState::ksUP && singleton.currentKeyStates[i] == KeyState::ksUP)
		{
			singleton.currentKeyStates[i] = KeyState::ksFREE;
		}
		singleton.previousKeyStates[i] = singleton.currentKeyStates[i];
	}
}