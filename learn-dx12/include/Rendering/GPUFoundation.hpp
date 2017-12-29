#pragma once

#include <pch.hpp>

#include <Utility\Application.hpp>
#include <Rendering\GPUEngine.hpp>
#include <Rendering\Data\GPUFrameResource.hpp>
#include <Rendering\Data\GPUUploadHeap.hpp>
#include <Rendering\Data\GPUDescriptorHeap.hpp>
#include <Rendering\Data\FrameGraph\GPUFrameGraph.hpp>

class GPUFoundation
{
public:
    GPUFoundation();
    GPUFoundation(Application& application);
    GPUFoundation(GPUFoundation const&) = delete;
    GPUFoundation(GPUFoundation&& rhs);

    GPUFoundation& operator=(GPUFoundation const&) = delete;
    GPUFoundation& operator=(GPUFoundation&&);

    ~GPUFoundation();

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
    Microsoft::WRL::ComPtr<IDXGIFactory1> DXGIFactory() { return dxgiFactory_; }

    GPUFrameResourceDescriptor& FinalRenderTargetViews() { return *finalRenderTargetViews_; }
    GPUFrameResourceDescriptor& FinalDepthSteniclViews() { return *finalDepthStencilViews_; }

    GPUFrameGraph& FrameGraph() { return *frameGraph_; }
    GPUDescriptorHeap& DescriptorHeap() { return *descriptorHeap_; }

    void CreateRootSignature(Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSignature, Microsoft::WRL::ComPtr<ID3D12RootSignature>& dest);
    void CreatePSO(Microsoft::WRL::ComPtr<ID3D12PipelineState>& dest, D3D12_GRAPHICS_PIPELINE_STATE_DESC* desc);

    static void CompileShader(LPCWSTR fileName, Microsoft::WRL::ComPtr<ID3DBlob>& dest, LPCSTR entryPoint, LPCSTR type);

private:
    static void GetHardwareAdapter(Microsoft::WRL::ComPtr<IDXGIAdapter1>& dest, Microsoft::WRL::ComPtr<IDXGIFactory1>& factory);
    void InitializeD3D12();
    void CreateGPUEngines();

    void CreateSwapChain(Application& application, IDXGIFactory* factory);
    void CreateFrameResources();
    void CreateDefaultDescriptorHeaps();

private:
    Microsoft::WRL::ComPtr<ID3D12Debug> debugController_;
    Microsoft::WRL::ComPtr<ID3D12Device> device_;
    Microsoft::WRL::ComPtr<IDXGIFactory1> dxgiFactory_;

    GPUEngine engines_[3];

    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain_;
    UINT64 currentFrame_ = 0U;

    GPUFrameGraph* frameGraph_ = nullptr;

    GPUFrameResource* renderTargetBuffers_ = nullptr;
    GPUFrameResource* depthStencilBuffers_ = nullptr;
    GPUFrameResourceDescriptor* finalRenderTargetViews_ = nullptr;
    GPUFrameResourceDescriptor* finalDepthStencilViews_ = nullptr;

    GPUDescriptorHeap* descriptorHeap_ = nullptr;
    int static constexpr RTV_HEAP_CAPACITY = 30;
    int static constexpr DSV_HEAP_CAPACITY = 30;
    int static constexpr CBV_SRV_UAV_CAPACITY = 30;
};