#pragma once

#include <Windows.h>

class InputManager
{
public:
    InputManager(HWND windowHandle);

    void Update();

    bool IsLeftButtonDown() const;
    bool IsLeftButtonPressed() const;
    bool IsLeftButtonReleased() const;

private:
    HWND hWnd;
    bool isLeftButtonDown;
    bool wasLeftButtonDown;
};

