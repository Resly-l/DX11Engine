#include "window.h"

#include "rendering/renderer.h"
#include "input/keyboard.h"
#include "input/mouse.h"

#include "imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Window::Window(LPCSTR title, LPCSTR iconPath, uint32_t uWidth, uint32_t uHeight)
	:
	hInstance(GetModuleHandleA(nullptr)),
	uWidth(uWidth),
	uHeight(uHeight)
{
	RegisterWindowClass(hInstance, iconPath);
	CreateAndDisplayWindow(hInstance, title);

	Renderer::Initialize(hWnd, uWidth, uHeight);

	Mouse::RegisterRawInputDevice(hWnd);
}

Window::~Window()
{
	DestroyWindow(hWnd);
	UnregisterClassA(className, nullptr);
}

bool Window::ProcessMessage() const
{
	MSG msg = {};

	while (PeekMessageA(&msg, nullptr, 0u, 0u, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}

		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	Keyboard::Update();
	Mouse::Update();

	return true;
}

void Window::Kill() const
{
	PostQuitMessage(0);
}

void Window::RegisterWindowClass(HINSTANCE hInstance, LPCSTR iconPath) const
{
	WNDCLASSEXA wc = {};

	wc.cbSize = sizeof(WNDCLASSEXA);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hIcon = (HICON)LoadImageA(0, iconPath, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = className;

	RegisterClassExA(&wc);
}

void Window::CreateAndDisplayWindow(HINSTANCE hInstance, LPCSTR title)
{
	RECT rtWindow = { 0, 0, (LONG)uWidth, (LONG)uHeight };
	AdjustWindowRect(&rtWindow, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	hWnd = CreateWindowExA(0, className, title, WS_SYSMENU | WS_MINIMIZEBOX,
		GetSystemMetrics(SM_CXSCREEN) / 2 - (rtWindow.right - rtWindow.left) / 2,
		GetSystemMetrics(SM_CYSCREEN) / 2 - (rtWindow.bottom - rtWindow.top) / 2,
		rtWindow.right - rtWindow.left, rtWindow.bottom - rtWindow.top,
		nullptr, nullptr, hInstance, nullptr
	);

	ShowWindow(hWnd, SW_SHOW);
}

LRESULT Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return S_OK;
	}

	if (Keyboard::HandleWM(hWnd, uMsg, wParam, lParam) || Mouse::HandleWM(hWnd, uMsg, wParam, lParam))
	{
		return DefWindowProcA(hWnd, uMsg, wParam, lParam);
	}

	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(S_OK);
		return S_OK;
	}

	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}