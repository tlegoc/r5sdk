//
// Created by theo on 29/12/2023.
//

#include "upscaling.h"

BOOL g_bIsUpscalingInitialized = false;
ID3D11Texture2D* p_upscalingInput = nullptr;
ID3D11ShaderResourceView* p_upscalingInput_SRV = nullptr;
BOOL bWasRunThisFrame = false;

#define CHECK_DX_RES(hr, call, error) \
	hr = call; \
	if (hr < S_OK) { \
		Msg(eDLL_T::SYSTEM_WARNING, error, hr); \
		return; \
	}

void Upscaling_Init()
{

	IDXGISwapChain* p_swapchain = D3D11SwapChain();
	DXGI_SWAP_CHAIN_DESC swapchainDesc;
	p_swapchain->GetDesc(&swapchainDesc);

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = swapchainDesc.BufferDesc.Width;
	desc.Height = swapchainDesc.BufferDesc.Height;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	HRESULT hr;
	CHECK_DX_RES(hr, D3D11Device()->CreateTexture2D(&desc, NULL, &p_upscalingInput), "Failed to initialize upscaling: upscalingInput texture could not be created %x\n")

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	CHECK_DX_RES(hr, D3D11Device()->CreateShaderResourceView(p_upscalingInput, &srvDesc, &p_upscalingInput_SRV), "Could not create shader resource view. %x\n")

	g_bIsUpscalingInitialized = true;
	Msg(eDLL_T::ENGINE, "Upscaling initialized.\n");
}

void Upscaling_Shutdown()
{

}


void Upscaling_Run()
{
	if (!g_bIsUpscalingInitialized) {

		Msg(eDLL_T::SYSTEM_WARNING, "Upscaling_Run() called without complete initialisation of upscaling.\n");
		return;
	}

	// Avoid running twice per frame
	if (bWasRunThisFrame) {

		Msg(eDLL_T::SYSTEM_WARNING, "Upscaling_Run() was already called this frame.\n");
		return;
	}
	bWasRunThisFrame = true;

	IDXGISwapChain* p_swapchain = D3D11SwapChain();
	ID3D11Texture2D* p_tempTex = nullptr;

	HRESULT hr;
	CHECK_DX_RES(hr, p_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&p_tempTex), "Upscaling error: could not retrieve swapchain texture. %x\n")

	if (p_tempTex == nullptr || p_upscalingInput == nullptr) return; 

	// Crashes seemingly randomly when called before Rui_Draw or vgui_baseui_interface Paint
	D3D11DeviceContext()->CopyResource(p_upscalingInput, p_tempTex);
}

void Upscaling_ResetFrameLock()
{
	bWasRunThisFrame = false;
}

void Upscaling_Resize(UINT newWidth, UINT newHeight)
{
	if (!g_bIsUpscalingInitialized) {

		Msg(eDLL_T::SYSTEM_WARNING, "Upscaling_Resize() called without complete initialisation of upscaling.\n");
		return;
	}

	Msg(eDLL_T::ENGINE, "Resizing upscaling texture: %u %u\n", newWidth, newHeight);
}

void Upscaling_DebugUI()
{
	if (!g_bImGuiInitialized && !g_bIsUpscalingInitialized) return;

	ImGui::Begin("Upscaling");
	IDXGISwapChain* p_swapchain = D3D11SwapChain();
	DXGI_SWAP_CHAIN_DESC desc;
	p_swapchain->GetDesc(&desc);
	if (ImGui::CollapsingHeader("Info"))
	{
		ImGui::Text("Viewport size: {%u, %u}", desc.BufferDesc.Width, desc.BufferDesc.Height);
	}

	if (ImGui::CollapsingHeader("Pre ui"))
	{
		ImGui::Text("Image pointer: %p", p_upscalingInput);
		ImGui::Text("Image SRV pointer: %p", p_upscalingInput_SRV);
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();
		if (p_upscalingInput_SRV != nullptr)
			ImGui::Image((void*)p_upscalingInput_SRV, ImVec2(vMax.x, vMax.x * 9.0f/16.0f));
	}

	ImGui::End();
}