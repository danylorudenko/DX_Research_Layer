#pragma once

#include <pch.hpp>

#include <Utility\Application.hpp>
#include <Rendering\GPUEngine.hpp>
#include <Data\FrameResource.hpp>
#include <Data\GPUResource.hpp>
#include <Data\GPUUploadHeap.hpp>

class GPUAccess
{
public:
    GPUAccess();
    GPUAccess(Application& application);
    GPUAccess(const ID3D12Device&) = delete;
    GPUAccess(GPUAccess&& rhs);

    GPUAccess& operator=(const GPUAccess&) = delete;
    GPUAccess& operator=(GPUAccess&&);

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

    ID3D12Resource* DepthStencilBuffer() const;
    ID3D12Resource* CurrentFramebuffer() const;

    D3D12_GPU_VIRTUAL_ADDRESS CurrentFramebufferGPUVirtualAddress() const;
    D3D12_GPU_VIRTUAL_ADDRESS DepthStencilBufferGPUVirtualAddress() const;

    D3D12_CPU_DESCRIPTOR_HANDLE CurrentRtvHandle() const;
    D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilHandle() const;

    UINT RtvIncrementalSize() const { return rtvDescriptorSize_; }
    UINT DsvIncrementalSize() const { return dsvDescriptorSize_; }
    UINT CbvSrvUavIncrementalSize() const { return cbv_srv_uavDescriptorSize_; }

    void CommitDefaultViewportScissorRects();

    void Present();

    void CreateRootSignature(Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSignature, Microsoft::WRL::ComPtr<ID3D12RootSignature>& dest);
    void CreateConstantBufferView(D3D12_CONSTANT_BUFFER_VIEW_DESC* desc, D3D12_CPU_DESCRIPTOR_HANDLE heapHandle);
    void CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_DESC* desc, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dest);

    void CreateGPUUploadHeap(GPUUploadHeap& dest, void const* data, std::size_t elementSize, bool isConstBuffer = false);
    void CreateGPUBuffer(GPUResource& dest, std::size_t size);
    void UpdateGPUResource(GPUResource& dest, std::size_t offset, const void* userData, std::size_t size);

    static void CompileShader(LPCWSTR fileName, Microsoft::WRL::ComPtr<ID3DBlob>& dest, LPCSTR entryPoint, LPCSTR type);

    void CreatePSO(Microsoft::WRL::ComPtr<ID3D12PipelineState>& dest, D3D12_GRAPHICS_PIPELINE_STATE_DESC* desc);

private:
    static void GetHardwareAdapter(IDXGIAdapter1** dest, IDXGIFactory1* factory);
    void InitializeD3D12();
    void CreateGPUWorkers();

    void CreateSwapChain(Application& application, IDXGIFactory* factory);
    void CreateFrameResources();
    void CreateDefaultDescriptorHeaps();
    void CreateDepthStencilBuffer();
    void SetViewportScissor();

    void CreateDepthStencilBufferView();

private:
    Microsoft::WRL::ComPtr<ID3D12Device> device_;
    Microsoft::WRL::ComPtr<IDXGIFactory1> dxgiFactory_;

    GPUEngine engines_[3];

    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain_;
    FrameResource frameResources_[SWAP_CHAIN_BUFFER_COUNT];
    UINT64 currentFrame_ = 0U;
    D3D12_VIEWPORT viewportRect_;
    D3D12_RECT scissorRect_;

    UINT rtvDescriptorSize_ = 0U;
    UINT dsvDescriptorSize_ = 0U;
    UINT cbv_srv_uavDescriptorSize_ = 0U;

    Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilBuffer_;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap_;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap_;
};