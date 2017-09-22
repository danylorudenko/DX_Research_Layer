#pragma once

#include <pch.hpp>

#include <Utility\Application.hpp>
#include <Utility\PerformanceTimer.hpp>
#include <Data\FrameResource.hpp>

class DirectAppDelegate : public Application::Delegate
{
public:
    virtual void start(Application& application) override;
    virtual void update(Application& application) override;
    virtual void shutdown(Application& application) override;

    //PerformanceTimer& Timer();

    static constexpr UINT WIDTH = 800;
    static constexpr UINT HEIGHT = 600;

    static constexpr UINT SWAP_CHAIN_BUFFER_COUNT = 3;

    static constexpr DXGI_FORMAT backBufferFormat_ = DXGI_FORMAT_R8G8B8A8_UNORM;
    static constexpr DXGI_FORMAT depthStencilBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

    ID3D12Device& Device();
    PerformanceTimer& Timer();
    void DisplayFrameTime(Application& application, float drawTime);

    D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetViewHandle() const;
    D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilViewHandle() const;

    void InitializeD3D12();
    void GetDescriptorSizes();
    void CreateDescriptorHeaps();
    void CreateMainCommandQueue();
    void CreateMainCommandList();
    void CreateFrameResources();
    void CreateDepthStencilBuffer();

    void CreateSwapChain(Application& application);
    void CreateDepthStencilBufferView(ID3D12GraphicsCommandList* startupCommandList);
    void SetViewportScissor();

    void CreateRootSignature();
    void CreatePipelineState();

    void LoadTriangleVertices(ID3D12GraphicsCommandList* startupCommandList);
    void LoadConstantBuffers();
    
    void Draw();
    void CustomAction();

    void Present();
    void WaitForFence(FrameResource& frameResource);

private:
    PerformanceTimer gameTimer_;

    Microsoft::WRL::ComPtr<IDXGIFactory1> dxgiFactory1_;

    Microsoft::WRL::ComPtr<ID3D12Device> device_;

    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;

    Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;
    

    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain_;
    FrameResource frameResources[SWAP_CHAIN_BUFFER_COUNT];

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap_;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap_;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> cbvHeap_;

    Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilBuffer_;

    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;

    UINT currentRenderBuffer = 0U;
    D3D12_VIEWPORT viewportRect_;
    D3D12_RECT scissorRect_;

    UINT rtvDescriptorSize_;
    UINT dsvDescriptorSize_;
    UINT cvb_srv_uavDescriptorSize_;

    UINT MSAA4xQuality_;


    // Additional data and resources.

    Microsoft::WRL::ComPtr<ID3D12Resource> constantBuffer_;
    SceneConstantBuffer constantBufferData_;
    UINT8* constantBufferMappedData_;

    Microsoft::WRL::ComPtr<ID3D12Resource> triangleVertices_;
    D3D12_VERTEX_BUFFER_VIEW triangleVerticesView_;

};