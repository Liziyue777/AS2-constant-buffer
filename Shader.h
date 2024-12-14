#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <windows.h>
#include "Core.h"
#include "ShaderPeflection.h"

class Shader {
public:
    ID3D11VertexShader* vertexShader = nullptr;
    ID3D11PixelShader* pixelShader = nullptr;
    ID3D11InputLayout* layout = nullptr;

    std::vector<ConstantBuffer> vsConstantBuffers;
    std::vector<ConstantBuffer> psConstantBuffers;
    std::map<std::string, int> textureBindPointsVS;
    std::map<std::string, int> textureBindPointsPS;

    // 读取着色器代码
    std::string loadShaderCode(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open shader file: " + filename);
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    // 加载顶点着色器并反射
    void loadVS(Core* core, const std::string& hlslCode, void (Shader::* layoutFunc)(ID3DBlob*, Core*) = nullptr) {
        ID3DBlob* shaderBlob = nullptr;
        ID3DBlob* errorBlob = nullptr;

        HRESULT hr = D3DCompile(
            hlslCode.c_str(),
            hlslCode.size(),
            NULL, NULL, NULL,
            "VS", "vs_5_0",
            0, 0,
            &shaderBlob,
            &errorBlob
        );

        if (FAILED(hr)) {
            if (errorBlob) {
                std::string errorMsg = (char*)errorBlob->GetBufferPointer();
                errorBlob->Release();
                throw std::runtime_error("Vertex Shader Compilation Error: " + errorMsg);
            }
            throw std::runtime_error("Failed to compile vertex shader.");
        }

        hr = core->device->CreateVertexShader(
            shaderBlob->GetBufferPointer(),
            shaderBlob->GetBufferSize(),
            NULL,
            &vertexShader
        );

        if (FAILED(hr)) {
            shaderBlob->Release();
            throw std::runtime_error("Failed to create vertex shader.");
        }

        // 创建输入布局
        if (layoutFunc) {
            (this->*layoutFunc)(shaderBlob, core);
        }

        // 反射顶点着色器常量缓冲区
        ConstantBufferReflection reflection;
        reflection.build(core, shaderBlob, vsConstantBuffers, textureBindPointsVS, ShaderStage::VertexShader);

        shaderBlob->Release();
    }

    // 加载像素着色器并反射
    void loadPS(Core* core, const std::string& hlslCode) {
        ID3DBlob* shaderBlob = nullptr;
        ID3DBlob* errorBlob = nullptr;

        HRESULT hr = D3DCompile(
            hlslCode.c_str(),
            hlslCode.size(),
            NULL, NULL, NULL,
            "PS", "ps_5_0",
            0, 0,
            &shaderBlob,
            &errorBlob
        );

        if (FAILED(hr)) {
            if (errorBlob) {
                std::string errorMsg = (char*)errorBlob->GetBufferPointer();
                errorBlob->Release();
                throw std::runtime_error("Pixel Shader Compilation Error: " + errorMsg);
            }
            throw std::runtime_error("Failed to compile pixel shader.");
        }

        hr = core->device->CreatePixelShader(
            shaderBlob->GetBufferPointer(),
            shaderBlob->GetBufferSize(),
            NULL,
            &pixelShader
        );

        if (FAILED(hr)) {
            shaderBlob->Release();
            throw std::runtime_error("Failed to create pixel shader.");
        }

        // 反射像素着色器常量缓冲区
        ConstantBufferReflection reflection;
        reflection.build(core, shaderBlob, psConstantBuffers, textureBindPointsPS,ShaderStage::PixelShader);

        shaderBlob->Release();
    }

    // 输入布局示例（可动态选择）
    void XInputLayout(ID3DBlob* compiledVertexShader, Core* core) {
        D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
            { "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOUR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        HRESULT hr = core->device->CreateInputLayout(
            layoutDesc,
            2,
            compiledVertexShader->GetBufferPointer(),
            compiledVertexShader->GetBufferSize(),
            &layout
        );

        if (FAILED(hr)) {
            throw std::runtime_error("Failed to create input layout.");
        }
    }

    // 绑定着色器及其常量缓冲区
    void bind(Core* core) {
        core->deviceContext->VSSetShader(vertexShader, NULL, 0);
        core->deviceContext->PSSetShader(pixelShader, NULL, 0);
        core->deviceContext->IASetInputLayout(layout);

        // 绑定顶点着色器的常量缓冲区
        for (size_t i = 0; i < vsConstantBuffers.size(); i++) {
            vsConstantBuffers[i].upload(core);
        }

        // 绑定像素着色器的常量缓冲区
        for (size_t i = 0; i < psConstantBuffers.size(); i++) {
            psConstantBuffers[i].upload(core);
        }
    }

    // 释放资源
    void release() {
        if (vertexShader) vertexShader->Release();
        if (pixelShader) pixelShader->Release();
        if (layout) layout->Release();
    }
};