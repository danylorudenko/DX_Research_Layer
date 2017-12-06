#pragma once

#include <pch.hpp>

#include <Rendering/Data/Resource/GPUResourceFrameSet.hpp>

class GPUUploadHeap : public GPUResourceFrameSet
{
public:
    GPUUploadHeap();
    GPUUploadHeap(int framesCount, ID3D12Device* device, void const* data, std::size_t size, D3D12_RESOURCE_DESC* resourceDesc, bool isConstBuffer = false);
    GPUUploadHeap(GPUUploadHeap const&) = delete;
    GPUUploadHeap(GPUUploadHeap&& rhs);

    GPUUploadHeap& operator=(GPUUploadHeap const&) = delete;
    GPUUploadHeap& operator=(GPUUploadHeap&& rhs);

    void Map(int frameIndex, void** dest, D3D12_RANGE* range) { ThrowIfFailed(resources_[frameIndex]->Map(0, range, dest)); }
    void Unmap(int frameIndex, D3D12_RANGE* range) { resources_[frameIndex]->Unmap(0, range); }
};