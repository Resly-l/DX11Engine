#include "mouse.h"
#include "imgui/imgui_impl_win32.h"

Mouse Mouse::singleton;

void Mouse::SetImGuiMouseUsage(bool bUseImGuiMouse)
{
	if (bUseImGuiMouse)
	{
		ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
	}
	else
	{
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
	}
}

void Mouse::SetCursorVisibility(bool bVisible)
{
	if (bVisible)
	{
		while (::ShowCursor(TRUE) < 0) {}
	}
	else
	{
		while (::ShowCursor(FALSE) >= 0) {}
	}
}

void Mouse::SetConfinement(bool bConfine)
{
	singleton.bConfined = bConfine;
}

void Mouse::SetCursorMovement(bool bMove)
{
	singleton.bFrozen = !bMove;
}

bool Mouse::IsCursorConfined()
{
	return singleton.bConfined;
}

bool Mouse::IsButtonFree(int iButton)
{
	return singleton.currentButtonStates[iButton] == ButtonState::bsFREE;
}

bool Mouse::IsButtonDown(int iButton)
{
	return singleton.currentButtonStates[iButton] == ButtonState::bsDOWN;
}

bool Mouse::IsButtonHold(int iButton)
{
	return singleton.currentButtonStates[iButton] == ButtonState::bsHOLD;
}

bool Mouse::IsButtonUp(int iButton)
{
	return singleton.currentButtonStates[iButton] == ButtonState::bsUP;
}

bool Mouse::IsWheelForward()
{
	return singleton.wheelState == WheelState::wsFORWARD;
}

bool Mouse::IsWheelBackward()
{
	return singleton.wheelState == WheelState::wsBACKWARD;
}

POINT Mouse::GetPosition()
{
	return singleton.ptPosition;
}

std::vector<POINT>& Mouse::GetRawDeltas()
{
	return singleton.rawDeltas[singleton.uActiveRawDelta == 1 ? 0 : 1];
}

void Mouse::RegisterRawInputDevice(HWND hWnd)
{
	RAWINPUTDEVICE rid = {};
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x02;
	rid.hwndTarget = hWnd;
	RegisterRawInputDevices(&rid, 1u, sizeof(rid));
}

bool Mouse::HandleWM(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		if (!ImGui::GetIO().WantCaptureMouse)
		{
			singleton.currentButtonStates[VK_LBUTTON] = ButtonState::bsDOWN;
		}
		break;
	case WM_RBUTTONDOWN:
		if (!ImGui::GetIO().WantCaptureMouse)
		{
			singleton.currentButtonStates[VK_RBUTTON] = ButtonState::bsDOWN;
		}
		break;
	case WM_MBUTTONDOWN:
		if (!ImGui::GetIO().WantCaptureMouse)
		{
			singleton.currentButtonStates[VK_MBUTTON] = ButtonState::bsDOWN;
		}
		break;
	case WM_XBUTTONDOWN:
		if (HIWORD(wParam) == 0x0001)
		{
			singleton.currentButtonStates[VK_XBUTTON1] = ButtonState::bsDOWN;
		}
		else
		{
			singleton.currentButtonStates[VK_XBUTTON2] = ButtonState::bsDOWN;
		}
		break;
	case WM_LBUTTONUP:
		singleton.currentButtonStates[VK_LBUTTON] = ButtonState::bsUP;
		break;
	case WM_RBUTTONUP:
		singleton.currentButtonStates[VK_RBUTTON] = ButtonState::bsUP;
		break;
	case WM_MBUTTONUP:
		singleton.currentButtonStates[VK_MBUTTON] = ButtonState::bsUP;
		break;
	case WM_XBUTTONUP:
		if (HIWORD(wParam) == 0x0001)
		{
			singleton.currentButtonStates[VK_XBUTTON1] = ButtonState::bsUP;
		}
		else
		{
			singleton.currentButtonStates[VK_XBUTTON2] = ButtonState::bsUP;
		}
		break;
	case WM_MOUSEWHEEL:
	{
		singleton.wheelState = GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? WheelState::wsFORWARD : WheelState::wsBACKWARD;
		singleton.bWheelMoved = true;
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (singleton.bConfined)
		{
			RECT rt;
			GetClientRect(hWnd, &rt);
			MapWindowPoints(hWnd, nullptr, (POINT*)&rt, 2);
			ClipCursor(&rt);
		}
		if (singleton.bFrozen)
		{
			POINT ptScreen = singleton.ptPosition;
			ClientToScreen(hWnd, &ptScreen);
			SetCursorPos(ptScreen.x, ptScreen.y);
		}
		else
		{
			ClipCursor(nullptr);
			POINTS pts = MAKEPOINTS(lParam);
			singleton.ptPosition.x = pts.x;
			singleton.ptPosition.y = pts.y;
		}
		break;
	}
	case WM_INPUT:
	{
		UINT size = 0u;
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));

		std::vector<BYTE> rawInputBuffer(size);
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, rawInputBuffer.data(), &size, sizeof(RAWINPUTHEADER));

		auto& ri = reinterpret_cast<const RAWINPUT&>(*(rawInputBuffer.data()));
		if (ri.header.dwType == RIM_TYPEMOUSE && (ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
		{
			singleton.rawDeltas[singleton.uActiveRawDelta].push_back({ ri.data.mouse.lLastX, ri.data.mouse.lLastY });
		}
		break;
	}
	default:
		return false;
	}

	return true;
}

void Mouse::Update()
{
	for (int i = 0; i < iNumButtons; i++)
	{
		if (singleton.previousButtonStates[i] == ButtonState::bsDOWN && singleton.currentButtonStates[i] == ButtonState::bsDOWN)
		{
			singleton.currentButtonStates[i] = ButtonState::bsHOLD;
		}
		else if (singleton.previousButtonStates[i] == ButtonState::bsUP && singleton.currentButtonStates[i] == ButtonState::bsUP)
		{
			singleton.currentButtonStates[i] = ButtonState::bsFREE;
		}
		singleton.previousButtonStates[i] = singleton.currentButtonStates[i];
	}

	if (!singleton.bWheelMoved)
	{
		singleton.wheelState = WheelState::wsIDLE;
	}
	singleton.bWheelMoved = false;

	singleton.uActiveRawDelta = singleton.uActiveRawDelta == 1u ? 0u : 1u;
	singleton.rawDeltas[singleton.uActiveRawDelta].clear();
}