#pragma once

#include <pch.hpp>

using Microsoft::WRL::ComPtr;

class UploadBuffer
{
public:
    UploadBuffer(
        ID3D12Device& device, 
        void* data, 
        std::size_t elementSize, 
        bool isConstBuffer = false, 
        std::size_t elementCount = 1);

    BYTE* MappedData();
    std::size_t BufferLength() const;


private:
    ComPtr<ID3D12Resource> uploadBuffer_ = nullptr;
    BYTE* mappedData_ = nullptr;

    void* data_ = nullptr;
    std::size_t elementSize_ = 0;
    std::size_t elementCount_ = 0;

};