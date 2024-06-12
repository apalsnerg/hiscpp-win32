#ifndef WINDOW_H
#define WINDOW_H
#define UNICODE

#pragma once
#include <tchar.h>
#include <windows.h>
#include <wingdi.h>
#include <iostream>
#include "roulette.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window {

    public:
        Window();
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        ~Window();

        bool processMessages();

        static void addMenus(HWND);
        
        Roulette& getRoulette();

        void drawText(HWND, wstring);

    private:
        HINSTANCE m_hInstance;
        HWND m_hwnd;
        Roulette m_roulette;
};

#endif
