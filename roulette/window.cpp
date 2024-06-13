#include "window.h"
#include <winuser.h>
#include <wingdi.h>
#include <time.h>
#include <string>
#include "player.h"
#include "roulette.h"

constexpr unsigned int FILE_MENU_BALANCE = 1;
constexpr unsigned int FILE_MENU_BET = 2;
constexpr unsigned int FILE_MENU_BALANCECHANGE = 3;
HWND ghParentWnd;

using namespace std;

/*
 * Switch to handle dispatched messages
 */
LRESULT CALLBACK WindowProc(HWND hParentWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_COMMAND:
            switch (wParam) {
                case FILE_MENU_BALANCE: { // Change the players's balance by 10 TODO: make a menu item where you can say how much you want to bet
                    Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hParentWnd, GWLP_USERDATA));
                    Roulette& roulette = pWindow->getRoulette();
                    Player& player = roulette.getPlayer();
                    player.updateBalance(10);
                    int balance = player.getBalance();
                    wstring balanceText = to_wstring(balance);
                    string name = player.getName();
                    wstring nameText(name.begin(), name.end());
                    pWindow->drawText(hParentWnd, nameText);
					break;
                }
                default:
					break;
            }
            break;
        case WM_CREATE: // Add menus and buttons etc when window is created
            ghParentWnd = hParentWnd;
            Window::addMenus(hParentWnd);
            Window::createTextBox();
            break;
        case WM_CLOSE: // Prompt user to confirm to close
            /* If user confirms to exit */
            if (MessageBox(hParentWnd, L"Are you sure you want to exit?", L"Exit", MB_OKCANCEL) == IDOK) { 
                DestroyWindow(hParentWnd);
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
            HDC hdc = BeginPaint(hParentWnd, &ps);

            // Do stuff

            FillRect(hdc, &ps.rcPaint, brush);

            EndPaint(hParentWnd, &ps);
            break;
        }
        default:
            return DefWindowProc(hParentWnd, uMsg, wParam, lParam); // Default behavior for event
    }
    return DefWindowProc(hParentWnd, uMsg, wParam, lParam); // Default behavior for event
}

LRESULT CALLBACK TextBoxProc(HWND hTextBoxWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_KEYDOWN:
            Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(ghParentWnd, GWLP_USERDATA));
            wstring text = pWindow->getTextBoxText();
            pWindow->drawText(ghParentWnd, text);
    }
    return DefWindowProc(hTextBoxWnd, uMsg, wParam, lParam);
}

/*
 * Constructor
 */
Window::Window(string name, int balance, int maxBet)
    : m_hParentInstance(GetModuleHandle(nullptr)), // Get handle to instance of window
      m_roulette(name, balance, maxBet)
{
    const wchar_t* CLASS_NAME = L"Roulette Window Class"; // Widechar name of class

    WNDCLASS wndClass      = {}; // Initialize class for window
    wndClass.lpszClassName = CLASS_NAME; // Set class name
    wndClass.hInstance     = m_hParentInstance; // Set class instance
    wndClass.hIcon         = LoadIcon(NULL, IDI_WINLOGO); // Set window icon
    wndClass.hCursor       = LoadCursor(NULL, IDC_ARROW); // Set cursor appearance
    wndClass.lpfnWndProc   = WindowProc; // Set window procedure

    RegisterClass(&wndClass); // Register class with set parameters

    /* Set window style */
    DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

    int width  = 1280; // Set base window width
    int height = 960; // Set base window height

    RECT rect{}; // Initialize rectangle
    rect.left   = 0; // Set window spawn distance from left side of screen
    rect.top    = 600; // Set window spawn distance from top of screen
    rect.right  = rect.left + width; // Set window width
    rect.bottom = rect.top + height; // Set window height

    AdjustWindowRect(&rect, style, false); // Adjust window size

    m_hParentWnd = CreateWindowExW( // Create the window process with our created parameters
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
        m_hParentInstance,
        NULL
    );

    SetWindowLongPtr(m_hParentWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    ShowWindow(m_hParentWnd, SW_SHOW); // Make window visible
};


/*
 * Destructor
 */
Window::~Window() {
    const wchar_t* CLASS_NAME = L"RouletteWindowClass"; // Get name of class to unregister (hardcoded here)

    UnregisterClass(CLASS_NAME, m_hParentInstance); // Unregister (destroy) the object with the class name and the hInstance for our window
};

void Window::drawText(HWND hParentWnd, wstring text) {
    HDC hdc = GetDC(hParentWnd);
    RECT rect;
    GetClientRect(hParentWnd, &rect);
    DrawText(hdc, text.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    ReleaseDC(hParentWnd, hdc);
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

void Window::addMenus(HWND hParentWnd) {
    HMENU hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"Modify");
    
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_BALANCE, L"Increase Balance");
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_BET, L"Set Pot");
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_BALANCECHANGE, L"Change Balance");
    AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);

    SetMenu(hParentWnd, hMenu);
}

HWND Window::createTextBox() {
    HINSTANCE hTextBoxInstance = GetModuleHandle(nullptr);
    HWND hTextWnd;

    const wchar_t* CLASS_NAME = L"TextBoxClass";

    WNDCLASS wndTXBXClass = {};
    wndTXBXClass.lpszClassName = CLASS_NAME;
    wndTXBXClass.hInstance = hTextBoxInstance;
    wndTXBXClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndTXBXClass.lpfnWndProc = TextBoxProc;
    
    RegisterClass(&wndTXBXClass);

    DWORD style = WS_CHILDWINDOW | WS_VISIBLE;

    int width = 420;
    int height = 240;

    RECT rect{};
    rect.left = 0;
    rect.top = 0;
    rect.right = rect.left + width;
    rect.bottom = rect.top + height;

    AdjustWindowRect(&rect, style, false);
    

    HMENU hTextBoxMenu = CreateMenu();
    AppendMenu(hTextBoxMenu, MF_STRING, 0, L"Test");
    
    hTextWnd = CreateWindowExW(
        0,
        L"EDIT",
        L"test",
        style,
        rect.left,
        rect.top,
        rect.right - rect.left,
        rect.bottom - rect.top,
        ghParentWnd,
        hTextBoxMenu,
        hTextBoxInstance,
        NULL
    );

    ShowWindow(hTextWnd, SW_SHOW);

    return hTextWnd;
}