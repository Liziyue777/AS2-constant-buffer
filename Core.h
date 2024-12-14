
#pragma once
#include <d3d11.h>
#include <stdexcept>
#include <cstring>
#include <D3D11.h>
#include <vector>
#include <dxgi1_6.h>

#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "DXGI.lib")

class Core {
public:
    ID3D11Device* device;
    ID3D11DeviceContext* deviceContext;
    IDXGISwapChain* swapChain;


    ID3D11RenderTargetView* backbufferRenderTargetView;
    ID3D11Texture2D* backbuffer;
    ID3D11DepthStencilView* depthStencilView;
    ID3D11Texture2D* depthbuffer;
    
    // some states
    ID3D11RasterizerState* rasterizerState;
    ID3D11DepthStencilState* depthStencilState;

    IDXGIAdapter1* GetAdapter() {
        IDXGIAdapter1* adapterf;
        std::vector<IDXGIAdapter1*> adapters;
        IDXGIFactory6* factory = NULL;
        
        CreateDXGIFactory(__uuidof(IDXGIFactory6), (void**)&factory);
        int i = 0;

        // 枚举适配器
        while (factory->EnumAdapters1(i, &adapterf) != DXGI_ERROR_NOT_FOUND) {
            adapters.push_back(adapterf);
            i++;
        }

        // 选择拥有最大显存的适配器
        long long maxVideoMemory = 0;
        int useAdapterIndex = 0;
        for (int i = 0; i < adapters.size(); i++) {
            DXGI_ADAPTER_DESC desc;
            adapters[i]->GetDesc(&desc);
            if (desc.DedicatedVideoMemory > maxVideoMemory) {
                maxVideoMemory = desc.DedicatedVideoMemory;
                useAdapterIndex = i;
            }
        }

        IDXGIAdapter1* adapter = adapters[useAdapterIndex];

        // 释放工厂资源
        factory->Release();

        // 返回最佳适配器
        return adapter;
    }

    void Initialize(HWND hwnd, int width, int height, bool window_fullscreen) {

        DXGI_SWAP_CHAIN_DESC sd;
        memset(&sd, 0, sizeof(DXGI_SWAP_CHAIN_DESC));
        sd.BufferCount = 1;
        sd.BufferDesc.Width = width;
        sd.BufferDesc.Height = height;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hwnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = window_fullscreen ? false : true;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        D3D_FEATURE_LEVEL fl;
        fl = D3D_FEATURE_LEVEL_11_0;

        IDXGIAdapter1* adapter = GetAdapter(); // 调用私有方法获取适配器
        
        D3D11CreateDeviceAndSwapChain(
            adapter,
            D3D_DRIVER_TYPE_UNKNOWN, 
            NULL,
            0,
            &fl,
            1,
            D3D11_SDK_VERSION,
            &sd,
            &swapChain,
            &device,
            NULL,
            &deviceContext
        );

        swapChain->SetFullscreenState(window_fullscreen, NULL);

        swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer);
        device->CreateRenderTargetView(backbuffer, NULL, &backbufferRenderTargetView);

        D3D11_TEXTURE2D_DESC dsvDesc;
        dsvDesc.Width = width;
        dsvDesc.Height = height;
        dsvDesc.MipLevels = 1;
        dsvDesc.ArraySize = 1;
        dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        dsvDesc.SampleDesc.Count = 1;
        dsvDesc.SampleDesc.Quality = 0;
        dsvDesc.Usage = D3D11_USAGE_DEFAULT;
        dsvDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        dsvDesc.CPUAccessFlags = 0;
        dsvDesc.MiscFlags = 0;

        device->CreateTexture2D(&dsvDesc, NULL, &depthbuffer);
        device->CreateDepthStencilView(depthbuffer, NULL, &depthStencilView);
        deviceContext->OMSetRenderTargets(1, &backbufferRenderTargetView, depthStencilView);

        D3D11_VIEWPORT viewport;
        viewport.Width = (float)width;
        viewport.Height = (float)height;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        deviceContext->RSSetViewports(1, &viewport);
    }

    void Clear() {
        float ClearColour[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
        deviceContext->ClearRenderTargetView(backbufferRenderTargetView, ClearColour);
        deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }

    void Present() {
        swapChain->Present(0, 0);
    }

    void SetupRasterizerState() {
        D3D11_RASTERIZER_DESC rsdesc;
        ZeroMemory(&rsdesc, sizeof(D3D11_RASTERIZER_DESC));
        rsdesc.FillMode = D3D11_FILL_SOLID;
        rsdesc.CullMode = D3D11_CULL_NONE;
        device->CreateRasterizerState(&rsdesc, &rasterizerState);
        deviceContext->RSSetState(rasterizerState);
    }

    void SetupDepthStencilState() {
        D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
        BOOL DepthEnable;
        D3D11_DEPTH_WRITE_MASK DepthWriteMask;
        D3D11_COMPARISON_FUNC DepthFunc;
        BOOL StencilEnable;
        UINT8 StencilReadMask;
        UINT8 StencilWriteMask;
        D3D11_DEPTH_STENCILOP_DESC FrontFace;
        D3D11_DEPTH_STENCILOP_DESC BackFace;
        device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
        deviceContext->OMSetDepthStencilState(depthStencilState, 0);
    }
};



