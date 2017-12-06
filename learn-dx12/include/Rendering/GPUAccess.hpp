#pragma once

#include <pch.hpp>

#include <Utility\Application.hpp>
#include <Rendering\GPUEngine.hpp>
#include <Rendering\Data\GPUResourceFrameSet.hpp>
#include <Rendering\Data\GPUUploadHeap.hpp>
#include <Rendering\Data\GPUDescriptorHeap.hpp>
#include <Rendering\Data\FrameGraph\GPUFrameGraph.hpp>

class GPUAccess
{
public:
    GPUAccess();
    GPUAccess(Application& application);
    GPUAccess(GPUAccess const&) = delete;
    GPUAccess(GPUAccess&& rhs);

    GPUAccess& operator=(GPUAccess const&) = delete;
    GPUAccess& operator=(GPUAccess&&);

    ~GPUAccess();

    static constexpr UINT WIDTH = 800;
    static constexpr UINT HEIGHT = 600;

    static constexpr UINT SWAP_CHAIN_BUFFER_COUNT = 3;

    static constexpr DXGI_FORMAT backBufferFormat_ = DXGI_FORMAT_R8G8B8A8_UNORM;
    static constexpr DXGI_FORMAT depthStencilBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

    template<GPU_ENGINE_TYPE TYPE> GPUEngine& Engine();
    
    template<> constexpr GPUEngine& Engine<GPU_ENGINE_TYPE_DIRECT>() { return engines_[0]; }
    template<> constexpr GPUEngine& Engine<GPU_ENGINE_TYPE_COPY>() { return engines_[1]; }
    template<> constexpr GPUEngine& Engine<GPU_ENGINE_TYPE_COMPUTE>() { return engines_[2]; }

    void ResetAll();

    Microsoft::WRL::ComPtr<ID3D12Device> Device() const { return device_; }
    Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain() { return swapChain_; }

    GPUResourceFrameSetDescriptor& FinalRenderTargetViews() { return *finalRenderTargetViews_; }
    GPUResourceFrameSetDescriptor& FinalDepthSteniclViews() { return *finalDepthStencilViews_; }

    GPUFrameGraph& FrameGraph() { return *frameGraph_; }
    GPUDescriptorHeap& DescriptorHeap() { return *descriptorHeap_; }

    void CommitDefaultViewportScissorRects();

    void CreateRootSignature(Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSignature, Microsoft::WRL::ComPtr<ID3D12RootSignature>& dest);
    void CreatePSO(Microsoft::WRL::ComPtr<ID3D12PipelineState>& dest, D3D12_GRAPHICS_PIPELINE_STATE_DESC* desc);

    static void CompileShader(LPCWSTR fileName, Microsoft::WRL::ComPtr<ID3DBlob>& dest, LPCSTR entryPoint, LPCSTR type);

private:
    static void GetHardwareAdapter(IDXGIAdapter1** dest, IDXGIFactory1* factory);
    void InitializeD3D12();
    void CreateGPUEngines();

    void CreateSwapChain(Application& application, IDXGIFactory* factory);
    void CreateFrameResources();
    void CreateDefaultDescriptorHeaps();
    void SetViewportScissor();

private:
    Microsoft::WRL::ComPtr<ID3D12Device> device_;
    Microsoft::WRL::ComPtr<IDXGIFactory1> dxgiFactory_;

    GPUEngine engines_[3];

    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain_;
    UINT64 currentFrame_ = 0U;

    GPUFrameGraph* frameGraph_ = nullptr;

    GPUResourceFrameSet* renderTargetBuffers_ = nullptr;
    GPUResourceFrameSet* depthStencilBuffers_ = nullptr;
    GPUResourceFrameSetDescriptor* finalRenderTargetViews_ = nullptr;
    GPUResourceFrameSetDescriptor* finalDepthStencilViews_ = nullptr;

    // Default surface description.
    D3D12_VIEWPORT viewportRect_;
    D3D12_RECT scissorRect_;


    GPUDescriptorHeap* descriptorHeap_ = nullptr;
    int static constexpr RTV_HEAP_CAPACITY = 30;
    int static constexpr DSV_HEAP_CAPACITY = 30;
    int static constexpr CBV_SRV_UAV_CAPACITY = 30;
};