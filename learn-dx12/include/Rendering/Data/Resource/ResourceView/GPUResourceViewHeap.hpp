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

    D3D12_GPU_DESCRIPTOR_HANDLE GPUHeapStart() const;
    D3D12_CPU_DESCRIPTOR_HANDLE CPUHeapStart() const;

    std::size_t Capacity() const;
    std::size_t DescriptorSize() const;

    void Reset();

private:
    std::size_t currentHeapOffset_ = 0;
    std::size_t heapCapacity_ = 0;
    std::size_t descriptorSize_ = 0;
    D3D12_DESCRIPTOR_HEAP_TYPE type_ = (D3D12_DESCRIPTOR_HEAP_TYPE)0;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapHandle_ = nullptr;
};