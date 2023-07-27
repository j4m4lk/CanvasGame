#include "ImGuiManager.h"


ImGuiManager::ImGuiManager(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context)
    : m_hwnd(hwnd), m_device(device), m_context(context)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(device, context);

    ImGui::StyleColorsDark();
}

void ImGuiManager::BeginFrame()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void ImGuiManager::RenderUI()
{
    ImGui::Begin("Missle Simulation");

	
	
	////Make keys and then assign them the values for the fucntions
	ImGui::Text(" Canvas Painter  ");
	if (ImGui::Button("Reset Scene")) {
		//Resetclicked = true;
	}
	//ImGui::Text(" Time scale : " + to_string(timescale).c_str());
	if (ImGui::Button("cam1")) {
	//	cam = 1;
	}
	if (ImGui::Button("cam2")) {
	//	cam = 2;
	}
	if (ImGui::Button("cam3")) {
		//activeCam = &cameras[2];
	}
	if (ImGui::Button("cam4")) {
		//activeCam = &cameras[3];
	}

	ImGui::Text("Timescale") ;

	
    ImGui::End();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

ImGuiManager::~ImGuiManager()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
