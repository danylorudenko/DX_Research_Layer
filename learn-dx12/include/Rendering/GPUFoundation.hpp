#pragma once

#include <pch.hpp>

#include <Foundation\Application.hpp>
#include <Rendering\GPUEngine.hpp>
#include <Rendering\GPUSwapChain.hpp>
#include <Rendering\Data\Resource\GPUStaticResourceAllocator.hpp>
#include <Rendering\Data\Resource\ResourceView\GPUResourceViewAllocator.hpp>
#include <Rendering\Data\Resource\GPUResourceViewContextTable.hpp>
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

    static constexpr UINT WIDTH = 800;
    static constexpr UINT HEIGHT = 600;

    static constexpr std::size_t SWAP_CHAIN_BUFFER_COUNT = 3;

    static constexpr DXGI_FORMAT backBufferFormat_ = DXGI_FORMAT_R8G8B8A8_UNORM;
    static constexpr DXGI_FORMAT depthStencilBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

    Microsoft::WRL::ComPtr<ID3D12Device>& Device() { return device_; }
    Microsoft::WRL::ComPtr<IDXGIFactory1> const& DXGIFactory() const { return dxgiFactory_; }
    GPUSwapChain& SwapChain() { return swapChain_; }

    template<GPU_ENGINE_TYPE TYPE> GPUEngine& Engine();
    template<> constexpr GPUEngine& Engine<GPU_ENGINE_TYPE_DIRECT>() { return engines_[0]; }
    template<> constexpr GPUEngine& Engine<GPU_ENGINE_TYPE_COPY>() { return engines_[1]; }
    template<> constexpr GPUEngine& Engine<GPU_ENGINE_TYPE_COMPUTE>() { return engines_[2]; }

    void ResetAll();


    GPUResourceHandle AllocDefaultResource(D3D12_RESOURCE_DESC const& desc, D3D12_RESOURCE_STATES initialState);
    GPUResourceHandle AllocUploadResource(D3D12_RESOURCE_DESC const& desc, D3D12_RESOURCE_STATES initialState);

    void SetDefaultDescriptorHeaps();

    GPUResourceViewHandle SwapChainRTV();
    GPUResourceViewHandle AllocRTV(std::size_t frames, GPUResourceHandle const* resources, D3D12_RENDER_TARGET_VIEW_DESC const* desc);
    GPUResourceViewHandle AllocDSV(std::size_t frames, GPUResourceHandle const* resources, D3D12_DEPTH_STENCIL_VIEW_DESC const* desc, D3D12_RESOURCE_STATES targetState);
    GPUResourceViewHandle AllocCBV(std::size_t frames, GPUResourceHandle const* resources, D3D12_CONSTANT_BUFFER_VIEW_DESC const* desc, D3D12_RESOURCE_STATES targetState);
    GPUResourceViewHandle AllocSRV(std::size_t frames, GPUResourceHandle const* resources, D3D12_SHADER_RESOURCE_VIEW_DESC const* desc, D3D12_RESOURCE_STATES targetState);
    GPUResourceViewHandle AllocUAV(std::size_t frames, GPUResourceHandle const* resources, D3D12_UNORDERED_ACCESS_VIEW_DESC const* desc, D3D12_RESOURCE_STATES targetState);
    GPUResourceViewTable BuildViewTable(std::size_t tableSize, GPUShaderVisibleResourceViewDesc const* descriptions);

    GPUFrameGraph& FrameGraph() { return frameGraph_; }

    void CreateRootSignature(Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSignature, Microsoft::WRL::ComPtr<ID3D12RootSignature>& dest);
    void CreatePSO(Microsoft::WRL::ComPtr<ID3D12PipelineState>& dest, D3D12_GRAPHICS_PIPELINE_STATE_DESC* desc);

    static void CompileShader(LPCWSTR fileName, Microsoft::WRL::ComPtr<ID3DBlob>& dest, LPCSTR entryPoint, LPCSTR type);

private:
    void InitializeD3D12();
    void CreateGPUEngines();
    GPUResourceViewHandle AllocSwapChainRTV(GPUSwapChain& swapChain, D3D12_RENDER_TARGET_VIEW_DESC const& desc);


private:
    Microsoft::WRL::ComPtr<ID3D12Debug> debugController_;
    Microsoft::WRL::ComPtr<ID3D12Device> device_;
    Microsoft::WRL::ComPtr<IDXGIFactory1> dxgiFactory_;

    GPUSwapChain swapChain_;
    GPUResourceViewHandle swapChainRTV_;

    GPUEngine engines_[3];

    UINT64 currentFrame_ = 0U;
    GPUFrameGraph frameGraph_;
    

    std::size_t static constexpr RTV_HEAP_CAPACITY = 30;
    std::size_t static constexpr DSV_HEAP_CAPACITY = 30;
    std::size_t static constexpr CBV_SRV_UAV_CAPACITY = 30;

    GPUResourceViewAllocator viewAllocator_;
    GPUResourceViewContextTable viewContextTable_;
    
    GPUStaticResourceAllocator staticResourceAllocator_;
};