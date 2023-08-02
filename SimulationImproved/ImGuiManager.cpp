#include "ImGuiManager.h"
#include "CubeData.h"
#include "NetworkStats.h"



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
    ImGui::Begin("Canvas Painter ");

	
	
	////Make keys and then assign them the values for the fucntions
	ImGui::Text(" Cube data ");
    ImGui::Text("Number of cubesmass : ");
     ImGui::Text("Network Stats");
     ImGui::Text(("Total players : " + std::to_string(totalPlayers)).c_str());
     ImGui::Text(("Server Connected: " + std::string(serverConnected ? "Yes" : "No")).c_str());
     ImGui::Text(("Client Connected: " + std::string(clientConnected ? "Yes" : "No")).c_str());
    

    // ImGui::Text(("Number of players: " + std::to_string(network.GetConnectedPlayers())).c_str());
   

	

	
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
