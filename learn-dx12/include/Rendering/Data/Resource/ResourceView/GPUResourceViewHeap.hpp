#pragma once

#include <pch.hpp>

#include <Rendering\GPUFoundation.hpp>

class GPUResourceViewHeap
{
public:
    GPUResourceViewHeap();
    GPUResourceViewHeap(GPUFoundation const& foundation, D3D12_DESCRIPTOR_HEAP_TYPE type, std::size_t capacity);
    GPUResourceViewHeap(GPUResourceViewHeap const& rhs) = delete;
    GPUResourceViewHeap(GPUResourceViewHeap&& rhs);

    GPUResourceViewHeap& operator=(GPUResourceViewHeap const& rhs) = delete;
    GPUResourceViewHeap& operator=(GPUResourceViewHeap&& rhs);

    D3D12_CPU_DESCRIPTOR_HANDLE ProvideNextHandle();

    void Reset();

private:
    std::size_t currentHeapOffset_ = 0;
    std::size_t heapCapacity_ = 0;
    std::size_t descriptorSize_ = 0;
    D3D12_DESCRIPTOR_HEAP_TYPE type_ = (D3D12_DESCRIPTOR_HEAP_TYPE)0;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapHandle_ = nullptr;
};