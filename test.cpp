#include "GEMLoader.h"
#include "Matrix.h"
#include <vector>
#include <stdexcept>
#include <cmath>
#include <iostream>

// 定义渲染上下文和画面大小
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// 初始化渲染上下文
void InitializeRendering() {
    // 可在此添加初始化窗口的代码（例如 SDL 或 OpenGL）
    std::cout << "Rendering context initialized." << std::endl;
}

// 绘制三角形
void DrawTriangle(const Vec3& v1, const Vec3& v2, const Vec3& v3, const Colour& colour) {
    // 在屏幕上画三角形的代码占位符
    std::cout << "Drawing triangle with vertices: "
        << v1.x << "," << v1.y << "," << v1.z << " - "
        << v2.x << "," << v2.y << "," << v2.z << " - "
        << v3.x << "," << v3.y << "," << v3.z
        << " and colour (" << colour.r << "," << colour.g << "," << colour.b << ")" << std::endl;
}

int main() {
    InitializeRendering();

    // 加载模型数据
    GEMLoader::GEMModelLoader loader;
    std::vector<GEMLoader::GEMMesh> meshes;
    loader.load("bunny.gem", meshes);

    // 转换模型顶点为屏幕空间
    std::vector<Vec3> vertexList;
    for (const auto& mesh : meshes) {
        for (const auto& index : mesh.indices) {
            const auto& gemVertex = mesh.verticesStatic[index].position;
            vertexList.push_back(Vec3(gemVertex.x, gemVertex.y, gemVertex.z));
        }
    }

    // 设置视图矩阵和投影矩阵
    Vec3 cameraPosition(0, 0, 5);
    Vec3 target(0, 0, 0);
    Vec3 up(0, 1, 0);
    Matrix view = Matrix().lookAt(cameraPosition, target, up);
    Matrix projection = Matrix().Perspective(3.14f / 4.0f, static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT, 0.1f, 100.0f);

    // 将顶点转换到屏幕空间并绘制
    for (size_t i = 0; i < vertexList.size(); i += 3) {
        Vec3 worldVertex1 = vertexList[i];
        Vec3 worldVertex2 = vertexList[i + 1];
        Vec3 worldVertex3 = vertexList[i + 2];

        // 转换到屏幕空间
        HomogeneousVector screenVertex1 = HomogeneousVector(worldVertex1.x, worldVertex1.y, worldVertex1.z, 1.0f);
        HomogeneousVector screenVertex2 = HomogeneousVector(worldVertex2.x, worldVertex2.y, worldVertex2.z, 1.0f);
        HomogeneousVector screenVertex3 = HomogeneousVector(worldVertex3.x, worldVertex3.y, worldVertex3.z, 1.0f);

        // 应用视图和投影变换
      //screenVertex1 = projection * view * screenVertex1;
      // screenVertex2 = projection * view * screenVertex2;
      // screenVertex3 = projection * view * screenVertex3;

      //  // 透视除法
      //  screenVertex1.PerspectiveDivide();
      //  screenVertex2.PerspectiveDivide();
      //  screenVertex3.PerspectiveDivide();

        // 将屏幕坐标映射到像素空间
        screenVertex1.x = (screenVertex1.x + 1.0f) * SCREEN_WIDTH * 0.5f;
        screenVertex1.y = (1.0f - screenVertex1.y) * SCREEN_HEIGHT * 0.5f;

        screenVertex2.x = (screenVertex2.x + 1.0f) * SCREEN_WIDTH * 0.5f;
        screenVertex2.y = (1.0f - screenVertex2.y) * SCREEN_HEIGHT * 0.5f;

        screenVertex3.x = (screenVertex3.x + 1.0f) * SCREEN_WIDTH * 0.5f;
        screenVertex3.y = (1.0f - screenVertex3.y) * SCREEN_HEIGHT * 0.5f;

        // 绘制三角形
        DrawTriangle(
            Vec3(screenVertex1.x, screenVertex1.y, screenVertex1.z),
            Vec3(screenVertex2.x, screenVertex2.y, screenVertex2.z),
            Vec3(screenVertex3.x, screenVertex3.y, screenVertex3.z),
            Colour(1.0f, 0.0f, 0.0f) // 设定颜色为红色
        );
    }

    return 0;
}