#include "InputManager.h"

InputManager::InputManager(HWND windowHandle) : hWnd(windowHandle), isLeftButtonDown(false), wasLeftButtonDown(false), mouseX(0), mouseY(0)
{
}

void InputManager::Update()
{
    wasLeftButtonDown = isLeftButtonDown;
    isLeftButtonDown = GetAsyncKeyState(VK_LBUTTON) & 0x8000;

    POINT cursorPos;
    GetCursorPos(&cursorPos);
    ScreenToClient(hWnd, &cursorPos);
    mouseX = cursorPos.x;
    mouseY = cursorPos.y;
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

int InputManager::GetMouseX() const
{
    return mouseX;
}

int InputManager::GetMouseY() const
{
    return mouseY;
}
