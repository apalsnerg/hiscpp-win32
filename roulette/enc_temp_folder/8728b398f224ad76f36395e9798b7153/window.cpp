#include "window.h"
#include <winuser.h>
#include <wingdi.h>
#include <time.h>
#include <string>
#include "player.h"
#include "roulette.h"

constexpr unsigned int FILE_MENU_BALANCE = 1;
constexpr unsigned int FILE_MENU_BET = 2;

using namespace std;

/*
 * Switch to handle dispatched messages
 */
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_COMMAND:
            switch (wParam) {
                case FILE_MENU_BALANCE: {
                    Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
                    Roulette& roulette = pWindow->getRoulette();
                    Player& player = roulette.getPlayer();
                    player.updateBalance(10);
                    int balance = player.getBalance();
                    wstring balanceText = to_wstring(balance);
                    pWindow->drawText(hWnd, balanceText);
					break;
                }
                default:
					break;
            }
            break;
        case WM_CREATE: // Add menus when window is created
            Window::addMenus(hWnd);
            break;
        case WM_CLOSE: // Prompt user to confirm to close
            /* If user confirms to exit */
            if (MessageBox(hWnd, L"Are you sure you want to exit?", L"Exit", MB_OKCANCEL) == IDOK) { 
                DestroyWindow(hWnd);
            }
            return 0; // Return 0 if user cancels exit to do nothing
            break;
        case WM_DESTROY: // User confirms to close, destroy window
            PostQuitMessage(0);
            return 0;
            break;
        case WM_LBUTTONDOWN: // Left mouse click event
            cout << "Left mouse click" << "\n";
            break;
        case WM_PAINT: // On paint event
        {
            PAINTSTRUCT ps;
            srand(time(NULL)); // Randomize seed so we can get random colours
			// Generate random RGB values between 1 and 255 (recolour breaks if (0,0,0), no idea why)
            int rVal = rand() % 255 + 1;
            int gVal = rand() % 255 + 1;
			int bVal = rand() % 255 + 1;
            HBRUSH brush = CreateSolidBrush(RGB(rVal, gVal, bVal)); // Create brush object with randomized RGB values
            HDC hdc = BeginPaint(hWnd, &ps);

            // Do stuff

            FillRect(hdc, &ps.rcPaint, brush);

            EndPaint(hWnd, &ps);
            break;
        }
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam); // Default behavior for event
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam); // Default behavior for event
}

/*
 * Constructor
 */
Window::Window()
    : m_hInstance(GetModuleHandle(nullptr)), // Get handle to instance of window
      m_roulette()
{
    const wchar_t* CLASS_NAME = L"Roulette Window Class"; // Widechar name of class

    WNDCLASS wndClass      = {}; // Initialize class for window
    wndClass.lpszClassName = CLASS_NAME; // Set class name
    wndClass.hInstance     = m_hInstance; // Set class instance
    wndClass.hIcon         = LoadIcon(NULL, IDI_WINLOGO); // Set window icon
    wndClass.hCursor       = LoadCursor(NULL, IDC_ARROW); // Set cursor appearance
    wndClass.lpfnWndProc   = WindowProc; // Set window procedure

    RegisterClass(&wndClass); // Register class with set parameters

    /* Set window style */
    DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

    int width  = 640; // Set base window width
    int height = 480; // Set base window height

    RECT rect{}; // Initialize rectangle
    rect.left   = 250; // Set window spawn distance from left side of screen
    rect.top    = 250; // Set window spawn distance from top of screen
    rect.right  = rect.left + width; // Set window width
    rect.bottom = rect.top + height; // Set window height

    AdjustWindowRect(&rect, style, false); // Adjust window size

    m_hwnd = CreateWindowExW( // Create the window process with our created parameters
        0,
        CLASS_NAME,
        L"Roulette",
        style,
        rect.left,
        rect.top,
        rect.right  - rect.left,
        rect.bottom - rect.top,
        NULL,
        NULL,
        m_hInstance,
        NULL
    );

    SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    ShowWindow(m_hwnd, SW_SHOW); // Make window visible
};


/*
 * Destructor
 */
Window::~Window() {
    const wchar_t* CLASS_NAME = L"RouletteWindowClass"; // Get name of class to unregister (hardcoded here)

    UnregisterClass(CLASS_NAME, m_hInstance); // Unregister (destroy) the object with the class name and the hInstance for our window
};

void Window::drawText(HWND hWnd, wstring text) {
    HDC hdc = GetDC(hWnd);
    RECT rect;
    GetClientRect(hWnd, &rect);
    DrawText(hdc, text.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    ReleaseDC(hWnd, hdc);
}

/*
* Getter for the Roulette member variable
*/
Roulette& Window::getRoulette() {
    return m_roulette;
}

/*
 * Message Loop
 * While the user does not close the window, process messages
 * (i.e. indirectly send them to WindowProc)
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

HMENU hMenu;

void Window::addMenus(HWND hWnd) {
    hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"Modify");
    
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_BALANCE, L"Increase Balance");
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_BET, L"Set Pot");
    AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);

    SetMenu(hWnd, hMenu);
}