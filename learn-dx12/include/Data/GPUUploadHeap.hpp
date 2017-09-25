#pragma once

#include <pch.hpp>

class GPUUploadHeap
{
public:
    GPUUploadHeap(ID3D12Device* device, void const* data, std::size_t elementSize, std::size_t elementCount = 1, bool isConstBuffer = false);

    BYTE* MappedData();
    std::size_t BufferLength() const;

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> uploadBuffer_ = nullptr;
    BYTE* mappedData_ = nullptr;

    void* data_ = nullptr;
    std::size_t elementSize_;
    std::size_t elementCount_;

};