#pragma once

#include <pch.hpp>

#include <Rendering\Data\GPUFrameResourceDescriptor.hpp>

class GPUDescriptorHeap
{
public:
    GPUDescriptorHeap();
    GPUDescriptorHeap(Microsoft::WRL::ComPtr<ID3D12Device> device, int rtvCapacity, int dsvCapacity, int CbvSrvUavCapacity);

    GPUDescriptorHeap(GPUDescriptorHeap const&) = delete;
    GPUDescriptorHeap(GPUDescriptorHeap&& rhs);

    GPUDescriptorHeap& operator=(GPUDescriptorHeap const&) = delete;
    GPUDescriptorHeap& operator=(GPUDescriptorHeap&& rhs);

    void Reset();

    GPUFrameResourceDescriptor AllocRtvLinear(std::vector<GPUResource*> resources,
                                              D3D12_RENDER_TARGET_VIEW_DESC* viewDesc,
                                              D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON,
                                              char const* semantics = "default",
                                              int frameCount = 1U);

    GPUFrameResourceDescriptor AllocDsvLinear(std::vector<GPUResource*> resources,
                                              D3D12_DEPTH_STENCIL_VIEW_DESC* viewDesc,
                                              D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON,
                                              char const* semantics = "default",
                                              int frameCount = 1U);

    GPUFrameResourceDescriptor AllocCbvLinear(std::vector<GPUResource*> resources,
                                              D3D12_CONSTANT_BUFFER_VIEW_DESC* viewDesc,
                                              D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON,
                                              char const* semantics = "default",
                                              int frameCount = 1U);

    GPUFrameResourceDescriptor AllocSrvLinear(std::vector<GPUResource*> resources,
                                              D3D12_SHADER_RESOURCE_VIEW_DESC* viewDesc,
                                              D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON,
                                              char const* semantics = "default",
                                              int frameCount = 1U);

    GPUFrameResourceDescriptor AllocUavLinear(std::vector<GPUResource*> resources,
                                              D3D12_UNORDERED_ACCESS_VIEW_DESC* viewDesc,
                                              D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON,
                                              char const* semantics = "default",
                                              int frameCount = 1U);


private:
    Microsoft::WRL::ComPtr<ID3D12Device> device_ = nullptr;
    
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapRtv_ = nullptr;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapDsv_ = nullptr;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapCbvSrvUav_ = nullptr;

    int heapCapacityRtv_ = 0ULL;
    int heapCapacityDsv_ = 0ULL;
    int heapCapacityCbvSrvUav_ = 0ULL;

    UINT heapRtvIncrementalSize_ = 0U;
    UINT heapDsvIncrementalSize_ = 0U;
    UINT heapCbvSrvUavIncrementalSize_ = 0U;

    int heapRtvLastDescriptorOffset_ = 0;
    int heapDsvLastDescriptorOffset_ = 0;
    int heapCbvSrvUavLastDescriptorOffset_ = 0;
};