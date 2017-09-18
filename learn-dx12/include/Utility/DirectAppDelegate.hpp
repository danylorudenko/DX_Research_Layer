#pragma once

#include <pch.hpp>

#include <Utility\Application.hpp>
#include <Utility\GameTimer.hpp>

class DirectAppDelegate : public Application::Delegate
{
public:
    virtual void start(Application& application) override;
    virtual void update(Application& application) override;
    virtual void shutdown(Application& application) override;

    //GameTimer& Timer();

    static constexpr UINT WIDTH = 800;
    static constexpr UINT HEIGHT = 600;

    static constexpr UINT SWAP_CHAIN_BUFFER_COUNT = 2;

    static constexpr DXGI_FORMAT backBufferFormat_ = DXGI_FORMAT_R8G8B8A8_UNORM;
    static constexpr DXGI_FORMAT depthStencilBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

    ID3D12Device& Device();

    D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetViewHandle() const;
    D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilViewHandle() const;

    void InitializeD3D12();
    void CreateFence();
    void GetDescriptorSizes();
    void CheckMXAA4xQuality();
    void CreateCommandObjects();
    void CreateSwapChain(Application& application);
    void CreateDescriptorHeaps();
    void CreateRenderTargetView();
    void CreateDepthStencilBufferView();
    void SetViewportScissor();

    void CreateRootSignature();
    void CreatePipelineState();
    void LoadTriangleVertices();
    void LoadConstantBuffers();
    
    void Present();
    void FlushCommandQueue();
    void WaitForGPUFinish();

    void Draw();

private:
    Microsoft::WRL::ComPtr<IDXGIFactory1> dxgiFactory1_;

    Microsoft::WRL::ComPtr<ID3D12Device> device_;
    Microsoft::WRL::ComPtr<ID3D12Fence> fence_;
    HANDLE fenceEvent_;
    UINT64 fenceValue_ = 0;

    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;

    Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;

    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain_;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap_;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap_;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> cbvHeap_;

    Microsoft::WRL::ComPtr<ID3D12Resource> swapChainBuffers_[SWAP_CHAIN_BUFFER_COUNT];
    Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilBuffer_;

    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
    Microsoft::WRL::ComPtr<ID3D12Resource> constantBuffer_;
    SceneConstantBuffer constantBufferData_;
    UINT8* consantBufferMappedData_;

    Microsoft::WRL::ComPtr<ID3D12Resource> triangleVertices_;
    D3D12_VERTEX_BUFFER_VIEW triangleVerticesView_;

    int currentBackBuffer = 0;
    D3D12_VIEWPORT viewportRect_;
    D3D12_RECT scissorRect_;

    UINT rtvDescriptorSize_;
    UINT dsvDescriptorSize_;
    UINT cvb_srv_uavDescriptorSize_;

    UINT MSAA4xQuality_;
};