#pragma once
#include "common.h"

class Window
{
public:
	Window(LPCSTR title, LPCSTR iconPath, uint32_t uWidth, uint32_t uHeight);
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	virtual ~Window();

public:
	bool ProcessMessage() const;
	void Kill() const;

private:
	void RegisterWindowClass(HINSTANCE hInstance, LPCSTR iconPath) const;
	void CreateAndDisplayWindow(HINSTANCE hInstance, LPCSTR title);
	static LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	inline static LPCSTR className = "window class";
	HINSTANCE hInstance;
	HWND hWnd;

	uint32_t uWidth;
	uint32_t uHeight;
};