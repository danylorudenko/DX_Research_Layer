#pragma once

#include <pch.hpp>

#include <Rendering/Data/Resource/GPUBuffer.hpp>

class GPUUploadHeapBuffer : public GPUBuffer
{
public:
    GPUUploadHeapBuffer();
    GPUUploadHeapBuffer(ID3D12Device* device, void const* data, D3D12_RESOURCE_DESC const* resourceDesc, std::size_t size, bool isConstBuffer);
    GPUUploadHeapBuffer(GPUUploadHeapBuffer const&) = delete;
    GPUUploadHeapBuffer(GPUUploadHeapBuffer&& rhs);

    GPUUploadHeapBuffer& operator=(GPUUploadHeapBuffer const&) = delete;
    GPUUploadHeapBuffer& operator=(GPUUploadHeapBuffer&& rhs);

protected:
    bool isConstBuffer_;
};