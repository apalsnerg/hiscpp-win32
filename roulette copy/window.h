#ifndef WINDOW_H
#define WINDOW_H

#pragma once
#include <tchar.h>
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window {
    public:
        Window();
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        ~Window();

        bool processMessages();

    private:
        HINSTANCE m_hInstance;
        HWND m_hWnd;
};

#endif
