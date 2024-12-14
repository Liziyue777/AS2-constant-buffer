#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include "Shader.h"
#include "Matrix.h"


struct Vertex
{
    Vec3 position;
    Colour colour;
};


class Triangle {
public:
    ID3D11Buffer* vertexBuffer = nullptr;
    ID3D11RasterizerState* rasterizerState = nullptr;
    ID3D11BlendState* blendState = nullptr;


    // 初始化顶点缓冲区和渲染状态
    void init(Core* core, Vertex* vertices, size_t N) {
        // 创建顶点缓冲区
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(Vertex) * static_cast<UINT>(N);
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA uploadData = {};
        uploadData.pSysMem = vertices;

        HRESULT hr = core->device->CreateBuffer(&bd, &uploadData, &vertexBuffer);
        if (FAILED(hr)) {
            throw std::runtime_error("Failed to create vertex buffer.");
        }

        // 配置光栅化状态
        D3D11_RASTERIZER_DESC rsdesc = {};
        rsdesc.FillMode = D3D11_FILL_SOLID;
        rsdesc.CullMode = D3D11_CULL_NONE;

        hr = core->device->CreateRasterizerState(&rsdesc, &rasterizerState);
        if (FAILED(hr)) {
            throw std::runtime_error("Failed to create rasterizer state.");
        }

        // 配置混合状态
        D3D11_BLEND_DESC blendDesc = {};
        ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
        blendDesc.RenderTarget[0].BlendEnable = FALSE;
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        hr = core->device->CreateBlendState(&blendDesc, &blendState);
        if (FAILED(hr)) {
            throw std::runtime_error("Failed to create blend state.");
        }
    }

    // 渲染三角形
    void render(ID3D11DeviceContext* devicecontext) {
        UINT offsets = 0;
        UINT strides = sizeof(Vertex);

        // 绑定顶点缓冲区
        devicecontext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);

        // 设置图元拓扑
        devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // 绑定光栅化状态
        devicecontext->RSSetState(rasterizerState);

        // 绘制三角形
        devicecontext->Draw(3, 0);
    }
};