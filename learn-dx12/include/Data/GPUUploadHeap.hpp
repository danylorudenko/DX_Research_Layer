#pragma once

#include <pch.hpp>

class GPUUploadHeap
{
public:
    GPUUploadHeap(ID3D12Device* device, void const* data, std::size_t size, bool isConstBuffer = false);
    GPUUploadHeap(const GPUUploadHeap&) = delete;
    GPUUploadHeap(GPUUploadHeap&& rhs);

    GPUUploadHeap& operator=(const GPUUploadHeap&) = delete;
    GPUUploadHeap& operator=(GPUUploadHeap&& rhs);

    ~GPUUploadHeap();

    ID3D12Resource* Get() const { return resource_.Get(); }
    std::size_t Size() const;
    BYTE* MappedData();

    D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAddress() const { return resource_->GetGPUVirtualAddress(); }

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> resource_ = nullptr;
    BYTE* mappedData_ = nullptr;
    std::size_t size_;

};