#pragma once

#include <pch.hpp>

using Microsoft::WRL::ComPtr;

class UploadBuffer
{
public:
    UploadBuffer(
        ID3D12Device& device, 
        void* data, 
        std::size_t dataSize, 
        bool isConstBuffer = false, 
        std::size_t dataCount = 1);

    ComPtr<ID3D12Resource> LoadToDefaultBuffer(ID3D12Device& device);

private:
    ComPtr<ID3D12Resource> uploadBuffer_;

    void* data_ = nullptr;
    std::size_t dataSize_ = 0;
    std::size_t dataCount_ = 0;

    BYTE* mappedData_ = nullptr;
};