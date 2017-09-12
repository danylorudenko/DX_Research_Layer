#pragma once

#include <pch.hpp>

#include <Utility\Application.hpp>

class DirectAppDelegate : public Application::Delegate
{
public:
    virtual void start(Application& application) override;
    virtual void update(Application& application) override;
    virtual void shutdown(Application& application) override;

    Microsoft::WRL::ComPtr<ID3D12Device> Device();

    void InitializeD3D12();
    void CreateFence();
    void GetDescriptorSizes();
    void CheckMXAA4xQuality();
    void CreateCommandObjects();
    void CreateSwapChain(Application& application);
    void CreateDescriptorHeaps();
    void CreateRenderTargetView();
    void CreateDepthStencilBufferView();


    int currentBackBuffer = 0;

    static constexpr UINT WIDTH = 800;
    static constexpr UINT HEIGHT = 600;

    static constexpr UINT SWAP_CHAIN_BUFFER_COUNT = 2;

    static constexpr DXGI_FORMAT backBufferFormat_ = DXGI_FORMAT_R8G8B8A8_UNORM;
    static constexpr DXGI_FORMAT depthStencilBufferFormat = DXGI_FORMAT_R24G8_TYPELESS;

private:
    Microsoft::WRL::ComPtr<IDXGIFactory1> dxgiFactory1_;

    Microsoft::WRL::ComPtr<ID3D12Device> device_;
    Microsoft::WRL::ComPtr<ID3D12Fence> fence_;

    Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;

    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain_;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap_;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap_;

    Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilBuffer_;

    UINT rtvDescriptorSize_;
    UINT dsvDescriptorSize_;
    UINT cvb_srv_uavDescriptorSize_;

    UINT MSAA4xQuality_;
};