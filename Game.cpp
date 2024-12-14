#include "window.h"
#include "Core.h"
#include "Mash.h"
#include "Shader.h"
#include "ShaderPeflection.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    // 创建窗口
    Window win;
    win.create(800, 600, "My Window");

    // 初始化核心
    Core core;
    core.Initialize(win.getHWND(), 800, 600, false);

    // 定义三角形顶点
    Vertex vertices[3] = {
        {{0, 1.0f, 0}, {0, 1.0f, 0}}, // 顶点 1
        {{-1.0f, -1.0f, 0}, {1.0f, 0, 0}}, // 顶点 2
        {{1.0f, -1.0f, 0}, {0, 0, 1.0f}}, // 顶点 3
    };

    // 初始化三角形
    Triangle triangle;
    triangle.init(&core, vertices, 3);

    // 初始化着色器
    Shader shader;
    std::string vsCode = shader.loadShaderCode("VStri.txt");
    std::string psCode = shader.loadShaderCode("PStri.txt");

    shader.loadVS(&core, vsCode, &Shader::XInputLayout);
    shader.loadPS(&core, psCode);

    // 添加常量缓冲区
    struct MatrixBuffer {
        float world[4][4]; // 世界矩阵
        float view[4][4];  // 视图矩阵
        float proj[4][4];  // 投影矩阵
    };

    MatrixBuffer matrixData;
    ConstantBuffer matrixBuffer;
    matrixBuffer.init(&core, sizeof(MatrixBuffer), 0, ShaderStage::VertexShader);

    // 设置默认矩阵数据（单位矩阵）
    memset(&matrixData, 0, sizeof(MatrixBuffer));
    for (int i = 0; i < 4; ++i) {
        matrixData.world[i][i] = 1.0f;
        matrixData.view[i][i] = 1.0f;
        matrixData.proj[i][i] = 1.0f;
    }

    // 游戏主循环
    MSG msg = {};
    while (true) {
        // 检测窗口消息
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // 更新逻辑（可以扩展）
        // 这里可以动态更新 `matrixData.world` 等数据

        // 上传矩阵数据到常量缓冲区
        matrixBuffer.update("world", &matrixData.world);
        matrixBuffer.update("view", &matrixData.view);
        matrixBuffer.update("proj", &matrixData.proj);
        matrixBuffer.upload(&core);

        // 渲染
        core.Clear();
        shader.bind(&core);
        triangle.render(core.deviceContext);
        core.Present();
    }

    // 释放资源
    triangle.vertexBuffer->Release();
    shader.release();
    matrixBuffer.free();

    return 0;
}