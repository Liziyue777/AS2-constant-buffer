#include "window.h"
Window* window = nullptr;
// 成员函数: 创建窗口
void Window::create(int window_width, int window_height, const std::string& window_name, float zoom) {
    WNDCLASSEX wc = {};
    hinstance = GetModuleHandle(NULL);
    name = window_name;

    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = Window::WndProc;  // 使用静态成员函数作为窗口过程函数
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hinstance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    std::wstring wname = std::wstring(window_name.begin(), window_name.end());
    wc.lpszClassName = wname.c_str();
    wc.cbSize = sizeof(WNDCLASSEX);
    RegisterClassEx(&wc);
    DWORD style;
    style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

    window = this;
    // 创建窗口
    hwnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        wname.c_str(),
        wname.c_str(),
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        window_width,
        window_height,
        NULL,
        NULL,
        hinstance,
        NULL
    );

    if (!hwnd) {
        exit(EXIT_FAILURE);
    }

    // 将当前类实例与窗口句柄关联
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

// 成员函数: 处理消息队列
void Window::ProcessMessages() {
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));

    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}


// 宏定义，用于获取鼠标事件的坐标
#define WINDOW_GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define WINDOW_GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // 使用静态变量访问当前窗口实例
    if (window) {
        switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;

        case WM_KEYDOWN:
            window->keys[(unsigned int)wParam] = true;
            return 0;

        case WM_KEYUP:
            window->keys[(unsigned int)wParam] = false;
            return 0;

        case WM_LBUTTONDOWN:
            window->updateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam));
            window->mouseButtons[0] = true;
            return 0;

        case WM_LBUTTONUP:
            window->updateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam));
            window->mouseButtons[0] = false;
            return 0;

        case WM_MOUSEMOVE:
            window->updateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam));
            return 0;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }

    // 如果没有实例，直接返回默认处理
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// 更新鼠标状态
void Window::updateMouse(int x, int y) {
    mousex = x;
    mousey = y;
}

extern "C" {
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}