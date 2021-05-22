#include "renderer.h"

#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

Renderer Renderer::instance;

Renderer::~Renderer()
{
	ReleaseImGui();
}

void Renderer::Initialize(HWND hwnd, uint32_t width, uint32_t height)
{
	instance.swapChainWidth = width;
	instance.swapChainHeight = height;

	CreateDeviceAndContext();
	CreateSwapChain(hwnd);
	CreateBackbuffer();

	SetViewport();

	InitializeImGui(hwnd);
}

void Renderer::BeginFrame(const Vector& clearColor)
{
	instance.pContext->ClearRenderTargetView(instance.pBackbuffer.Get(), &clearColor.x);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Renderer::EndFrame()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	instance.pSwapChain->Present(instance.useVSync, 0);
}

void Renderer::ResizeSwapChain(uint32_t width, uint32_t height)
{
	instance.swapChainWidth = width;
	instance.swapChainHeight = height;

	instance.pContext->ClearState();

	// manual releasing
	instance.pBackbuffer = nullptr;

	DXGI_MODE_DESC md = {};
	md.Width = width;
	md.Height = height;
	md.RefreshRate = { 0, 1 };
	md.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	md.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	md.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	instance.pSwapChain->ResizeTarget(&md);
	instance.pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_UNKNOWN, 0);

	CreateBackbuffer();
	SetViewport();
}

void Renderer::SetFullscreenState(bool bFullscreen)
{
	instance.pSwapChain->SetFullscreenState(bFullscreen, nullptr);
}

void Renderer::CreateSwapChain(HWND hwnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = instance.swapChainWidth;
	sd.BufferDesc.Height = instance.swapChainHeight;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hwnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;

	Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
	instance.pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

	Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	DXGI_ADAPTER_DESC desc = {};
	dxgiAdapter->GetDesc(&desc);

	Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
	dxgiFactory->CreateSwapChain(instance.pDevice.Get(), &sd, &instance.pSwapChain);
	dxgiFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);
}

void Renderer::CreateDeviceAndContext()
{
	uint32_t flags = 0;

#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, nullptr, 0, D3D11_SDK_VERSION, &instance.pDevice, {}, &instance.pContext)))
	{
		MessageBoxA(nullptr, "failed to create graphics device", "FATAL ERROR", MB_ICONERROR | MB_OK);
		PostQuitMessage(0);
	}
}

void Renderer::CreateBackbuffer()
{
	ComPtr<ID3D11Resource> pResource;
	instance.pSwapChain->GetBuffer(0u, __uuidof(ID3D11Resource), &pResource);
	if (FAILED(instance.pDevice->CreateRenderTargetView(pResource.Get(), nullptr, &instance.pBackbuffer)))
	{
		MessageBoxA(nullptr, "failed to create backbuffer", "FATAL ERROR", MB_ICONERROR | MB_OK);
		PostQuitMessage(0);
	}

	instance.pContext->OMSetRenderTargets(1u, instance.pBackbuffer.GetAddressOf(), nullptr);
}

void Renderer::SetViewport()
{
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = (float)instance.swapChainWidth;
	viewport.Height = (float)instance.swapChainHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	instance.pContext->RSSetViewports(1, &viewport);
}

void Renderer::InitializeImGui(HWND hwnd)
{
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	imnodes::CreateContext();
	imnodes::StyleColorsDark();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(instance.pDevice.Get(), instance.pContext.Get());

	ImGui::GetIO().Fonts->AddFontFromFileTTF("../asset/fonts/nnsreb.ttf", instance.imGuiFontSize);
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void Renderer::ReleaseImGui()
{
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();

	imnodes::DestroyContext();
	ImGui::DestroyContext();
}