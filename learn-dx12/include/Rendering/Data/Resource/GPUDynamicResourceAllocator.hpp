#pragma once

#include <pch.hpp>

#include <Rendering\GPUFoundation.hpp>
#include <Rendering\Data\Resource\GPUResource.hpp>

class GPUDynamicResourcePtr
{
public:
    GPUDynamicResourcePtr();
    GPUDynamicResourcePtr(GPUDynamicResourcePtr const& rhs);
    GPUDynamicResourcePtr(GPUDynamicResourcePtr&& rhs);

    GPUDynamicResourcePtr& operator=(GPUDynamicResourcePtr const& rhs);
    GPUDynamicResourcePtr& operator=(GPUDynamicResourcePtr&& rhs);

    std::size_t chunkIndex_ = 0;
};

class GPUDynamicResourceAllocator
{
public:
    GPUDynamicResourceAllocator();
    GPUDynamicResourceAllocator(GPUFoundation& foundation, D3D12_HEAP_TYPE type, std::size_t size, std::size_t chunkSize = 256);
    GPUDynamicResourceAllocator(GPUDynamicResourceAllocator const&) = delete;
    GPUDynamicResourceAllocator(GPUDynamicResourceAllocator&& rhs);

    GPUDynamicResourceAllocator& operator=(GPUDynamicResourceAllocator const&) = delete;
    GPUDynamicResourceAllocator& operator=(GPUDynamicResourceAllocator&& rhs);

    D3D12_HEAP_TYPE Type() const;

    GPUDynamicResourcePtr AllocDynamicResource(D3D12_RESOURCE_DESC const& resourceDesc, D3D12_RESOURCE_STATES initialState = D3D12_RESOURCE_STATE_COMMON);
    void FreeDynamicResource(GPUDynamicResourcePtr ptr);

private:
    Microsoft::WRL::ComPtr<ID3D12Heap> defaultHeap_ = nullptr;
    D3D12_HEAP_TYPE heapType_ = (D3D12_HEAP_TYPE)0;
    std::size_t heapSize_ = 0;
    std::size_t chunkSize_ = 0;
    
    std::size_t headChunk_;
    std::vector<std::size_t> chunksMetadata_;

    std::vector<std::unique_ptr<GPUResource>> placedResources_;
};