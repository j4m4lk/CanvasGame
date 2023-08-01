#pragma once
#include <windows.h>
#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")
#include "imgui\\imgui.h"
#include "imgui\\imgui_impl_dx11.h"
#include "imgui\\imgui_impl_win32.h"
#include <windows.h>
#include <string>



class ImGuiManager
{
public:
    ImGuiManager(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context);
    void BeginFrame();
    void RenderUI();
    ~ImGuiManager();


private:
    HWND m_hwnd;
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_context;
};
