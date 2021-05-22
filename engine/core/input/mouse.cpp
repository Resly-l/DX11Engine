#include "mouse.h"
#include "imgui/imgui_impl_win32.h"

Mouse Mouse::instance;

void Mouse::SetImGuiMouseUsage(bool useImGuiMouse)
{
	if (useImGuiMouse)
	{
		ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
	}
	else
	{
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
	}
}

void Mouse::SetCursorVisibility(bool visible)
{
	if (visible)
	{
		while (::ShowCursor(TRUE) < 0) {}
	}
	else
	{
		while (::ShowCursor(FALSE) >= 0) {}
	}
}

void Mouse::SetConfinement(bool confine)
{
	instance.confined = confine;
}

void Mouse::SetCursorMovement(bool freeze)
{
	instance.frozen = !freeze;
}

bool Mouse::IsCursorConfined()
{
	return instance.confined;
}

bool Mouse::IsButtonFree(int button)
{
	return instance.currentButtonStates[button] == ButtonState::bsFREE;
}

bool Mouse::IsButtonDown(int button)
{
	return instance.currentButtonStates[button] == ButtonState::bsDOWN;
}

bool Mouse::IsButtonHold(int button)
{
	return instance.currentButtonStates[button] == ButtonState::bsHOLD;
}

bool Mouse::IsButtonUp(int button)
{
	return instance.currentButtonStates[button] == ButtonState::bsUP;
}

bool Mouse::IsWheelForward()
{
	return instance.wheelState == WheelState::wsFORWARD;
}

bool Mouse::IsWheelBackward()
{
	return instance.wheelState == WheelState::wsBACKWARD;
}

POINT Mouse::GetPosition()
{
	return instance.position;
}

std::vector<POINT>& Mouse::GetRawDeltas()
{
	return instance.rawDeltas[instance.activeRawDeltaIndex == 1 ? 0 : 1];
}

void Mouse::RegisterRawInputDevice(HWND hwnd)
{
	RAWINPUTDEVICE rid = {};
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x02;
	rid.hwndTarget = hwnd;
	RegisterRawInputDevices(&rid, 1u, sizeof(rid));
}

bool Mouse::HandleWM(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		if (!ImGui::GetIO().WantCaptureMouse)
		{
			instance.currentButtonStates[VK_LBUTTON] = ButtonState::bsDOWN;
		}
		break;
	case WM_RBUTTONDOWN:
		if (!ImGui::GetIO().WantCaptureMouse)
		{
			instance.currentButtonStates[VK_RBUTTON] = ButtonState::bsDOWN;
		}
		break;
	case WM_MBUTTONDOWN:
		if (!ImGui::GetIO().WantCaptureMouse)
		{
			instance.currentButtonStates[VK_MBUTTON] = ButtonState::bsDOWN;
		}
		break;
	case WM_XBUTTONDOWN:
		if (HIWORD(wParam) == 0x0001)
		{
			instance.currentButtonStates[VK_XBUTTON1] = ButtonState::bsDOWN;
		}
		else
		{
			instance.currentButtonStates[VK_XBUTTON2] = ButtonState::bsDOWN;
		}
		break;
	case WM_LBUTTONUP:
		instance.currentButtonStates[VK_LBUTTON] = ButtonState::bsUP;
		break;
	case WM_RBUTTONUP:
		instance.currentButtonStates[VK_RBUTTON] = ButtonState::bsUP;
		break;
	case WM_MBUTTONUP:
		instance.currentButtonStates[VK_MBUTTON] = ButtonState::bsUP;
		break;
	case WM_XBUTTONUP:
		if (HIWORD(wParam) == 0x0001)
		{
			instance.currentButtonStates[VK_XBUTTON1] = ButtonState::bsUP;
		}
		else
		{
			instance.currentButtonStates[VK_XBUTTON2] = ButtonState::bsUP;
		}
		break;
	case WM_MOUSEWHEEL:
	{
		instance.wheelState = GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? WheelState::wsFORWARD : WheelState::wsBACKWARD;
		instance.wheelMoved = true;
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (instance.confined)
		{
			RECT clientRect;
			GetClientRect(hwnd, &clientRect);
			MapWindowPoints(hwnd, nullptr, (POINT*)&clientRect, 2);
			ClipCursor(&clientRect);
		}
		if (instance.frozen)
		{
			POINT screen = instance.position;
			ClientToScreen(hwnd, &screen);
			SetCursorPos(screen.x, screen.y);
		}
		else
		{
			ClipCursor(nullptr);
			POINTS pts = MAKEPOINTS(lParam);
			instance.position.x = pts.x;
			instance.position.y = pts.y;
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
			instance.rawDeltas[instance.activeRawDeltaIndex].push_back({ ri.data.mouse.lLastX, ri.data.mouse.lLastY });
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
	for (int i = 0; i < numButtons; i++)
	{
		if (instance.previousButtonStates[i] == ButtonState::bsDOWN && instance.currentButtonStates[i] == ButtonState::bsDOWN)
		{
			instance.currentButtonStates[i] = ButtonState::bsHOLD;
		}
		else if (instance.previousButtonStates[i] == ButtonState::bsUP && instance.currentButtonStates[i] == ButtonState::bsUP)
		{
			instance.currentButtonStates[i] = ButtonState::bsFREE;
		}
		instance.previousButtonStates[i] = instance.currentButtonStates[i];
	}

	if (!instance.wheelMoved)
	{
		instance.wheelState = WheelState::wsIDLE;
	}
	instance.wheelMoved = false;

	instance.activeRawDeltaIndex = instance.activeRawDeltaIndex == 1u ? 0u : 1u;
	instance.rawDeltas[instance.activeRawDeltaIndex].clear();
}