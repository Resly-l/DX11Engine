#pragma once
#include "common.h"

class Mouse
{
	friend class Window;

private:
	enum class ButtonState
	{
		bsFREE,
		bsDOWN,
		bsHOLD,
		bsUP
	};

	enum class WheelState
	{
		wsIDLE,
		wsFORWARD,
		wsBACKWARD
	};

private:
	static Mouse singleton;

	static constexpr int iNumButtons = 7;

	ButtonState previousButtonStates[iNumButtons] = {};
	ButtonState currentButtonStates[iNumButtons] = {};

	WheelState wheelState = WheelState::wsIDLE;

	std::vector<POINT> rawDeltas[2];
	size_t uActiveRawDelta = 0;

	POINT ptPosition = {};

	bool bWheelMoved = false;
	bool bConfined = false;
	bool bFrozen = false;

private:
	Mouse() = default;

public:
	static void SetImGuiMouseUsage(bool bUseImGuiMouse);
	static void SetCursorVisibility(bool bVisible);
	static void SetConfinement(bool bConfine);
	static void SetCursorMovement(bool bMove);

	static bool IsCursorConfined();

	static bool IsButtonFree(int iButton);
	static bool IsButtonDown(int iButton);
	static bool IsButtonHold(int iButton);
	static bool IsButtonUp(int iButton);

	static bool IsWheelForward();
	static bool IsWheelBackward();

	static POINT GetPosition();
	static std::vector<POINT>& GetRawDeltas();

private:
	static void RegisterRawInputDevice(HWND hWnd);
	static bool HandleWM(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void Update();
};