#pragma once
#include "common.h"

class Window
{
private:
	inline static LPCSTR className = "window class";

	HINSTANCE hinstance;
	HWND hwnd;

	uint32_t width;
	uint32_t height;

public:
	Window(LPCSTR title, LPCSTR iconPath, uint32_t width, uint32_t height);
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	virtual ~Window();

public:
	bool ProcessMessage() const;
	void Kill() const;

private:
	void RegisterWindowClass(HINSTANCE hinstance, LPCSTR iconPath) const;
	void CreateAndDisplayWindow(HINSTANCE hinstance, LPCSTR title);
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};