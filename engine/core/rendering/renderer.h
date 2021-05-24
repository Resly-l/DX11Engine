#pragma once
#include "common.h"

class Renderer
{
private:
	Renderer() = default;

public:
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	~Renderer();

public:
	static void Initialize(HWND hwnd, uint32_t width, uint32_t height);

	static ID3D11Device* GetDevice() { return instance.pDevice.Get(); };
	static ID3D11DeviceContext* GetContext() { return instance.pContext.Get(); };

	static ID3D11RenderTargetView* GetBackbuffer() { return instance.pBackbuffer.Get(); }

	static uint32_t GetSwapChainWidth() { return instance.swapChainWidth; };
	static uint32_t GetSwapChainHeight() { return instance.swapChainHeight; };

	static void BeginFrame(const Vector& clearColor = {});
	static void EndFrame();

	static void ResizeSwapChain(uint32_t width, uint32_t height);
	static void SetFullscreenState(bool useFullscreen);

private:
	static void CreateDeviceAndContext();
	static void CreateSwapChain(HWND hwnd);
	static void CreateBackbuffer();
	static void SetViewport();

	static void InitializeImGui(HWND hwnd);
	static void ReleaseImGui();

private:
	static Renderer instance;

	ComPtr<ID3D11Device> pDevice;
	ComPtr<ID3D11DeviceContext> pContext;
	ComPtr<IDXGISwapChain> pSwapChain;
	ComPtr<ID3D11RenderTargetView> pBackbuffer;

	uint32_t swapChainWidth;
	uint32_t swapChainHeight;

	bool useVSync = true;
	float imGuiFontSize = 15.0f;
};