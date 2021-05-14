#include "renderer.h"

#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

Renderer Renderer::singleton;

Renderer::~Renderer()
{
	ReleaseImGui();
}

void Renderer::Initialize(HWND hWnd, uint32_t uWidth, uint32_t uHeight)
{
	singleton.uSwapChainWidth = uWidth;
	singleton.uSwapChainHeight = uHeight;

	CreateDeviceAndContext();
	CreateSwapChain(hWnd);
	CreateBackbuffer();

	SetViewport();

	InitializeImGui(hWnd);
}

void Renderer::BeginFrame(const Vector& clearColor)
{
	singleton.pContext->ClearRenderTargetView(singleton.pBackbuffer.Get(), &clearColor.x);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Renderer::EndFrame()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	singleton.pSwapChain->Present(singleton.bVSync, 0);
}

void Renderer::ResizeSwapChain(uint32_t uWidth, uint32_t uHeight)
{
	singleton.uSwapChainWidth = uWidth;
	singleton.uSwapChainHeight = uHeight;

	singleton.pContext->ClearState();

	// manual releasing
	singleton.pBackbuffer = nullptr;

	DXGI_MODE_DESC md = {};
	md.Width = uWidth;
	md.Height = uHeight;
	md.RefreshRate = { 0, 1 };
	md.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	md.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	md.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	singleton.pSwapChain->ResizeTarget(&md);
	singleton.pSwapChain->ResizeBuffers(1, uWidth, uHeight, DXGI_FORMAT_UNKNOWN, 0);

	CreateBackbuffer();
	SetViewport();
}

void Renderer::SetFullscreenState(bool bFullscreen)
{
	singleton.pSwapChain->SetFullscreenState(bFullscreen, nullptr);
}

void Renderer::CreateSwapChain(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = singleton.uSwapChainWidth;
	sd.BufferDesc.Height = singleton.uSwapChainHeight;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;

	Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
	singleton.pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

	Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	DXGI_ADAPTER_DESC desc = {};
	dxgiAdapter->GetDesc(&desc);

	Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
	dxgiFactory->CreateSwapChain(singleton.pDevice.Get(), &sd, &singleton.pSwapChain);
	dxgiFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);
}

void Renderer::CreateDeviceAndContext()
{
	uint32_t flags = 0;

#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, nullptr, 0, D3D11_SDK_VERSION, &singleton.pDevice, {}, &singleton.pContext)))
	{
		MessageBoxA(nullptr, "failed to create graphics device", "FATAL ERROR", MB_ICONERROR | MB_OK);
		PostQuitMessage(0);
	}
}

void Renderer::CreateBackbuffer()
{
	ComPtr<ID3D11Resource> pResource;
	singleton.pSwapChain->GetBuffer(0u, __uuidof(ID3D11Resource), &pResource);
	if (FAILED(singleton.pDevice->CreateRenderTargetView(pResource.Get(), nullptr, &singleton.pBackbuffer)))
	{
		MessageBoxA(nullptr, "failed to create backbuffer", "FATAL ERROR", MB_ICONERROR | MB_OK);
		PostQuitMessage(0);
	}

	singleton.pContext->OMSetRenderTargets(1u, singleton.pBackbuffer.GetAddressOf(), nullptr);
}

void Renderer::SetViewport()
{
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = (float)singleton.uSwapChainWidth;
	viewport.Height = (float)singleton.uSwapChainHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	singleton.pContext->RSSetViewports(1, &viewport);
}

void Renderer::InitializeImGui(HWND hWnd)
{
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	imnodes::CreateContext();
	imnodes::StyleColorsDark();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(singleton.pDevice.Get(), singleton.pContext.Get());

	ImGui::GetIO().Fonts->AddFontFromFileTTF("../asset/fonts/nnsreb.ttf", singleton.fImGuiFontSize);
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void Renderer::ReleaseImGui()
{
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();

	imnodes::DestroyContext();
	ImGui::DestroyContext();
}