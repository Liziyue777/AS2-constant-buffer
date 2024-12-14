#pragma once

#include <Windows.h>
#include <string>

class Window{
public:
    HWND hwnd;                // 窗口句柄
    HINSTANCE hinstance;      // 实例句柄
    std::string name;         // 窗口名称
    bool keys[256] = { false }; // 按键状态
    int mousex = 0;           // 鼠标 X 坐标
    int mousey = 0;           // 鼠标 Y 坐标
    bool mouseButtons[3] = { false }; // 鼠标按键状态
    

    void create(int window_width, int window_height, const std::string& window_name, float zoom = 1.0f);
    void ProcessMessages();
    // void handleInput(float deltaTime);
     // 静态窗口过程函数
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    // 用于更新鼠标状态的成员函数
    void updateMouse(int x, int y);

    HWND getHWND() const {
        return hwnd;
    }
};





