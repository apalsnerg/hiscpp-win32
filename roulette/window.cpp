#include "window.h"
#include <wingdi.h>
#include <time.h>

/*
 * Callback to handle window events
 */
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE: // Sent when window is closed
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY: // Sent when window is closed
            PostQuitMessage(0);
            return 0;
            break;
        case WM_LBUTTONDOWN: // Left mouse click
        {
            PAINTSTRUCT ps;
            srand(time(NULL));
            int rVal = rand() % 256;
            int gVal = rand() % 256;
			int bVal = rand() % 256;
            HBRUSH brush = CreateSolidBrush(RGB(rVal, gVal, bVal)); // Create brush object with randomized RGB values
            HDC hdc = BeginPaint(hwnd, &ps);

            // Do stuff

            FillRect(hdc, &ps.rcPaint, brush);

            EndPaint(hwnd, &ps);
            break;
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam); // Default behavior for event
    }
    return 0;
    
}

/*
 * Constructor
 */
Window::Window() 
    : m_hInstance(GetModuleHandle(nullptr)) // Get handle to instance of window
{
    const wchar_t* CLASS_NAME = L"Roulette Window Class"; // Widechar name of class

    WNDCLASS wndClass = {}; // Initialize class for window
    wndClass.lpszClassName = CLASS_NAME; // Set class name
    wndClass.hInstance = m_hInstance; // Set class instance
    wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO); // Set window icon
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); // Set cursor appearance
    wndClass.lpfnWndProc = WindowProc; // Set window procedure

    RegisterClass(&wndClass); // Register class with set parameters

    DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU; // Set window style

    int width = 640; // Set base window width
    int height = 480; // Set base window height

    RECT rect; // Initialize rectangle
    rect.left = 250; // Set window spawn distance from left side of screen
    rect.top = 250; // Set window spawn distance from top of screen
    rect.right = rect.left + width; // Set window width
    rect.bottom = rect.top + height; // Set window height

    AdjustWindowRect(&rect, style, false); // Adjust window size

    m_hwnd = CreateWindowExW( // Create the window process with our created parameters
        0,
        CLASS_NAME,
        L"Roulette",
        style,
        rect.left,
        rect.top,
        rect.right - rect.left,
        rect.bottom - rect.top,
        NULL,
        NULL,
        m_hInstance,
        NULL
    );

    ShowWindow(m_hwnd, SW_SHOW); // Make window visible
};

/*
 * Destructor
 */
Window::~Window() {
    const wchar_t* CLASS_NAME = L"RouletteWindowClass"; // Get name of class to unregister (hardcoded here)

    UnregisterClass(CLASS_NAME, m_hInstance); // Unregister (destroy) the object with the class name and the hInstance for our window
};
/*
 * Handle messages 
 */
bool Window::processMessages() {
    MSG msg = {};

    while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) {

        if (msg.message == WM_QUIT) {
            return false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
};
