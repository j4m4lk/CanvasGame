#include "ImGuiManager.h"
#include "CubeData.h"


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
     ImGui::Text("Number of cubes mass : ");
     ImGui::Text("Number of cubes mass taken : ");
     ImGui::Text("Network Stats");
     ImGui::Text("Total players : ");
     ImGui::Text("Player No =  ");
    // ImGui::Text(("Number of players: " + std::to_string(network.GetConnectedPlayers())).c_str());
   

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
