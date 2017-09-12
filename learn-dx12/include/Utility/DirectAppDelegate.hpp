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

private:
    void InitializeD3D12();
    void CreateFence();
    void GetDescriptorSizes();
    void CheckMXAA4xQuality();
    void CreateCommandObjects();
    void CreateSwapChain();

private:
    Microsoft::WRL::ComPtr<ID3D12Device> device_;
    Microsoft::WRL::ComPtr<ID3D12Fence> fence_;

    Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;

    UINT rtvDescriptorSize_;
    UINT dsvDescriptorSize_;
    UINT cvb_srv_uavDescriptorSize_;

    UINT MSAA4xQuality_;
    
    DXGI_FORMAT backBufferFormat_ = DXGI_FORMAT_R8G8B8A8_UNORM;
    DXGI_FORMAT depthStencilBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
};