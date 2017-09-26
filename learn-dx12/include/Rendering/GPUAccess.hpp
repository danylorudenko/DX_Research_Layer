#pragma once

#include <pch.hpp>

#include <Utility\Application.hpp>
#include <Rendering\GPUWorker.hpp>
#include <Data\FrameResource.hpp>
#include <Data\GPUResource.hpp>
#include <Data\GPUUploadHeap.hpp>

class GPUAccess
{
public:
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

    template<GPU_WORKER_TYPE TYPE> GPUWorker& Worker();
    
    template<> constexpr GPUWorker& Worker<GPU_WORKER_TYPE_DIRECT>() { return *workers_[0]; }
    template<> constexpr GPUWorker& Worker<GPU_WORKER_TYPE_COPY>() { return *workers_[1]; }
    template<> constexpr GPUWorker& Worker<GPU_WORKER_TYPE_COMPUTE>() { return *workers_[2]; }

    UINT RtvIncrementalSize() const { return rtvDescriptorSize_; }
    UINT DsvIncrementalSize() const { return dsvDescriptorSize_; }
    UINT CbvSrvUavIncrementalSize() const { return cbv_srv_uavDescriptorSize_; }

    void Begin();
    void End();
    void Present();

    ID3D12GraphicsCommandList& Commit() { return Worker<GPU_WORKER_TYPE_DIRECT>().Commit(); }

    void CreateRootSignature(Microsoft::WRL::ComPtr<ID3DBlob> dest);
    void CreateConstantBufferView(D3D12_CONSTANT_BUFFER_VIEW_DESC* desc, D3D12_CPU_DESCRIPTOR_HANDLE heapHandle);
    void CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_DESC* desc, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dest);

    void CreateGPUUploadHeap(GPUUploadHeap** dest, void const* data, std::size_t elementSize, std::size_t elementsCount = 1, bool isConstBuffer = false);
    void CreateGPUBuffer(GPUResource** dest, std::size_t size);
    void UpdateGPUResource(GPUResource& dest, std::size_t offset, const void* userData, std::size_t size);

    static void CompileShader(LPWSTR fileName, ID3DBlob* dest, LPCSTR entryPoint, LPCSTR type);

    void CreatePSO(Microsoft::WRL::ComPtr<ID3D12PipelineState>& dest, D3D12_GRAPHICS_PIPELINE_STATE_DESC* desc);

private:

    void InitializeD3D12();
    void CreateGPUWorkers();

    void CreateSwapChain(Application& application, IDXGIFactory* factory);
    void CreateFrameResources();
    void CreateDefaultDescriptorHeaps();
    void CreateDepthStencilBuffer();
    void SetViewportScissor();

    void CreateDepthStencilBufferView();

private:
    static int usageNumber_;
    
    Microsoft::WRL::ComPtr<ID3D12Device> device_;
    Microsoft::WRL::ComPtr<IDXGIFactory1> dxgiFactory_;

    GPUWorker* workers_[3];

    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain_;
    FrameResource frameResources[SWAP_CHAIN_BUFFER_COUNT];
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