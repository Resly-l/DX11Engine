#pragma once
#include "common.h"

class Renderer
{
private:
	static Renderer singleton;

	ComPtr<ID3D11Device> pDevice;
	ComPtr<ID3D11DeviceContext> pContext;
	ComPtr<IDXGISwapChain> pSwapChain;
	ComPtr<ID3D11RenderTargetView> pBackbuffer;

	uint32_t uSwapChainWidth;
	uint32_t uSwapChainHeight;

	bool bVSync = true;
	float fImGuiFontSize = 15.0f;

private:
	Renderer() = default;

public:
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	~Renderer();

public:
	static void Initialize(HWND hWnd, uint32_t uWidth, uint32_t uHeight);

	static ID3D11Device* GetDevice() { return singleton.pDevice.Get(); };
	static ID3D11DeviceContext* GetContext() { return singleton.pContext.Get(); };

	static ID3D11RenderTargetView* GetBackbuffer() { return singleton.pBackbuffer.Get(); }

	static uint32_t GetSwapChainWidth() { return singleton.uSwapChainWidth; };
	static uint32_t GetSwapChainHeight() { return singleton.uSwapChainHeight; };

	static void BeginFrame(const Vector& clearColor = {});
	static void EndFrame();

	static void ResizeSwapChain(uint32_t uWidth, uint32_t uHeight);
	static void SetFullscreenState(bool bFullscreen);

private:
	static void CreateDeviceAndContext();
	static void CreateSwapChain(HWND hWnd);
	static void CreateBackbuffer();
	static void SetViewport();

	static void InitializeImGui(HWND hWnd);
	static void ReleaseImGui();
};