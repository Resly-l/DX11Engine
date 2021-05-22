#include "window.h"

#include "rendering/renderer.h"
#include "input/keyboard.h"
#include "input/mouse.h"

#include "imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

Window::Window(LPCSTR title, LPCSTR iconPath, uint32_t width, uint32_t height)
	:
	hinstance(GetModuleHandleA(nullptr)),
	width(width),
	height(height)
{
	RegisterWindowClass(hinstance, iconPath);
	CreateAndDisplayWindow(hinstance, title);

	Renderer::Initialize(hwnd, width, height);

	Mouse::RegisterRawInputDevice(hwnd);
}

Window::~Window()
{
	DestroyWindow(hwnd);
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

void Window::RegisterWindowClass(HINSTANCE hinstance, LPCSTR iconPath) const
{
	WNDCLASSEXA wc = {};

	wc.cbSize = sizeof(WNDCLASSEXA);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hinstance;
	wc.hIcon = (HICON)LoadImageA(0, iconPath, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = className;

	RegisterClassExA(&wc);
}

void Window::CreateAndDisplayWindow(HINSTANCE hinstance, LPCSTR title)
{
	RECT windowRect = { 0, 0, (LONG)width, (LONG)height };
	AdjustWindowRect(&windowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	hwnd = CreateWindowExA(0, className, title, WS_SYSMENU | WS_MINIMIZEBOX,
		GetSystemMetrics(SM_CXSCREEN) / 2 - (windowRect.right - windowRect.left) / 2,
		GetSystemMetrics(SM_CYSCREEN) / 2 - (windowRect.bottom - windowRect.top) / 2,
		windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
		nullptr, nullptr, hinstance, nullptr
	);

	ShowWindow(hwnd, SW_SHOW);
}

LRESULT Window::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
	{
		return S_OK;
	}

	if (Keyboard::HandleWM(hwnd, msg, wParam, lParam) || Mouse::HandleWM(hwnd, msg, wParam, lParam))
	{
		return DefWindowProcA(hwnd, msg, wParam, lParam);
	}

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(S_OK);
		return S_OK;
	}

	return DefWindowProcA(hwnd, msg, wParam, lParam);
}