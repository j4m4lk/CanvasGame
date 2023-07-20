#include "InputManager.h"
InputManager::InputManager(HWND windowHandle) : hWnd(windowHandle), isLeftButtonDown(false), wasLeftButtonDown(false)
{

}

void InputManager::Update()
{
    wasLeftButtonDown = isLeftButtonDown;

    // Check if left mouse button is currently down
    isLeftButtonDown = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;

    // Debug messages
    OutputDebugStringA("Left Button Down: ");
    OutputDebugStringA(isLeftButtonDown ? "true\n" : "false\n");

}

bool InputManager::IsLeftButtonDown() const
{
    return isLeftButtonDown;
}

bool InputManager::IsLeftButtonPressed() const
{
    return isLeftButtonDown && !wasLeftButtonDown;
}

bool InputManager::IsLeftButtonReleased() const
{
    return !isLeftButtonDown && wasLeftButtonDown;
}