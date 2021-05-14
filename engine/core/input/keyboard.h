#pragma once
#include "common.h"

class Keyboard
{
	friend class Window;
private:
	enum class KeyState
	{
		ksFREE,
		ksDOWN,
		ksHOLD,
		ksUP
	};

private:
	static Keyboard singleton;

	static constexpr int iNumKeys = 256;

	KeyState previousKeyStates[iNumKeys] = {};
	KeyState currentKeyStates[iNumKeys] = {};

	bool bAutoRepeat = false;

private:
	Keyboard() = default;

public:
	static bool IsKeyFree(unsigned char key);
	static bool IsKeyDown(unsigned char key);
	static bool IsKeyHold(unsigned char key);
	static bool IsKeyUp(unsigned char key);

	static bool IsAutoRepeatEnabled();
	static void SetAutoRepeat(bool bAutoRepeat);

private:
	static bool HandleWM(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void Update();
};