#pragma once

#include <pch.hpp>

#include <Rendering\Data\GPUFrameResourceView.hpp>

class GPUResourceViewAllocator
{
public:
    GPUResourceViewAllocator();
    GPUResourceViewAllocator(Microsoft::WRL::ComPtr<ID3D12Device> device, int rtvCapacity, int dsvCapacity, int CbvSrvUavCapacity);

    GPUResourceViewAllocator(GPUResourceViewAllocator const&) = delete;
    GPUResourceViewAllocator(GPUResourceViewAllocator&& rhs);

    GPUResourceViewAllocator& operator=(GPUResourceViewAllocator const&) = delete;
    GPUResourceViewAllocator& operator=(GPUResourceViewAllocator&& rhs);

    void Reset();

    GPUFrameResourceView AllocRtvLinear(GPUResourceSet* resources,
                                              D3D12_RENDER_TARGET_VIEW_DESC* viewDesc,
                                              D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON,
                                              char const* semantics = "default",
                                              int frameCount = 1U);

    GPUFrameResourceView AllocDsvLinear(GPUResourceSet* resources,
                                              D3D12_DEPTH_STENCIL_VIEW_DESC* viewDesc,
                                              D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON,
                                              char const* semantics = "default",
                                              int frameCount = 1U);

    GPUFrameResourceView AllocCbvLinear(GPUResourceSet* resources,
                                              D3D12_CONSTANT_BUFFER_VIEW_DESC* viewDesc,
                                              D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON,
                                              char const* semantics = "default",
                                              int frameCount = 1U);

    GPUFrameResourceView AllocSrvLinear(GPUResourceSet* resources,
                                              D3D12_SHADER_RESOURCE_VIEW_DESC* viewDesc,
                                              D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON,
                                              char const* semantics = "default",
                                              int frameCount = 1U);

    GPUFrameResourceView AllocUavLinear(GPUResourceSet* resources,
                                              D3D12_UNORDERED_ACCESS_VIEW_DESC* viewDesc,
                                              D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON,
                                              char const* semantics = "default",
                                              int frameCount = 1U);


    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> HeapRtv() { return heapRtv_; }
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> HeapDsv() { return heapDsv_; }
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> HeapCbvSrvUav() { return heapCbvSrvUav_; }

private:
    Microsoft::WRL::ComPtr<ID3D12Device> device_ = nullptr;
    
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapRtv_ = nullptr;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapDsv_ = nullptr;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapCbvSrvUav_ = nullptr;

    int heapCapacityRtv_ = 0;
    int heapCapacityDsv_ = 0;
    int heapCapacityCbvSrvUav_ = 0;

    UINT heapRtvIncrementalSize_ = 0;
    UINT heapDsvIncrementalSize_ = 0;
    UINT heapCbvSrvUavIncrementalSize_ = 0;

    int heapRtvLastDescriptorOffset_ = 0;
    int heapDsvLastDescriptorOffset_ = 0;
    int heapCbvSrvUavLastDescriptorOffset_ = 0;
};