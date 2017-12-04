#pragma once

#include <pch.hpp>

#include <Rendering\Data\GPUResource.hpp>

class GPUResourceSet
{
public:
    GPUResourceSet();
    GPUResourceSet(int framesCount, ID3D12Device* device, std::size_t size, D3D12_RESOURCE_DESC* resourceDesc, D3D12_RESOURCE_STATES state);
    GPUResourceSet(int framesCount, std::size_t resourceSize, Microsoft::WRL::ComPtr<ID3D12Resource>* resources, D3D12_RESOURCE_STATES state);
    GPUResourceSet(GPUResourceSet const&) = delete;
    GPUResourceSet(GPUResourceSet&& rhs);

    GPUResourceSet& operator=(GPUResourceSet const&) = delete;
    GPUResourceSet& operator=(GPUResourceSet&& rhs);
    
    ID3D12Resource* Get(int frameIndex) const { return resources_[frameIndex].Get(); }
    std::size_t Size() const { return size_; };
    std::size_t Capacity() const { return capacity_; }
    int FramesCount() const { return framesCount_; }

    D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAddress(int frameIndex) const { return gpuAddresses_[frameIndex]; }

    void CreateResources(int framesCount, ID3D12Device* device, std::size_t size, D3D12_RESOURCE_DESC* resourceDesc, D3D12_RESOURCE_STATES initialState);
    void UpdateData(int frameIndex, ID3D12GraphicsCommandList* commandList, std::size_t offsetInDest, GPUResourceSet& src, int srcFrameIndex, std::size_t offsetInSrc, std::size_t numBytes);

    void Transition(int frameIndex, ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES state);
    D3D12_RESOURCE_STATES State(int frameIndex) const;

protected:
    std::size_t resourceCount = 0;
    std::vector<GPUResource*> resources_;
};