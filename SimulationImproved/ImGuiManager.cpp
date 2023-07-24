//#include "ImGuiManager.h"
//#include "ImGuiManager.h"
//
//ImGuiManager::ImGuiManager(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context)
//    : m_hwnd(hwnd), m_device(device), m_context(context)
//{
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGuiIO& io = ImGui::GetIO(); (void)io;
//
//    ImGui_ImplWin32_Init(hwnd);
//    ImGui_ImplDX11_Init(device, context);
//
//    ImGui::StyleColorsDark();
//}
//
//void ImGuiManager::BeginFrame()
//{
//    ImGui_ImplDX11_NewFrame();
//    ImGui_ImplWin32_NewFrame();
//    ImGui::NewFrame();
//}
//
//void ImGuiManager::RenderUI()
//{
//    ImGui::Begin("Missle Simulation");
//    // Add ImGui UI components here...
//    ImGui::End();
//    ImGui::Render();
//    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
//}
//
//ImGuiManager::~ImGuiManager()
//{
//    ImGui_ImplDX11_Shutdown();
//    ImGui_ImplWin32_Shutdown();
//    ImGui::DestroyContext();
//}
