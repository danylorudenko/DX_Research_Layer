#pragma once

#include <pch.hpp>

#include <Rendering\GPUFoundation.hpp>
#include <Rendering\Data\Resource\GPUResource.hpp>


class GPUResourceAllocator
{
public:
    GPUResourceAllocator();
    GPUResourceAllocator(GPUFoundation const& foundation);

    GPUResourceAllocator(GPUResourceAllocator const&) = delete;
    GPUResourceAllocator(GPUResourceAllocator&& rhs);

    GPUResourceAllocator& operator=(GPUResourceAllocator const&) = delete;
    GPUResourceAllocator& operator=(GPUResourceAllocator&& rhs);

    GPUResourceDirectID DefaultAlloc(D3D12_RESOURCE_DESC& resourceDesc, D3D12_RESOURCE_STATES initialState);
    std::size_t ProvideNextResourceHandle();

private:
    GPUFoundation const* foundation_;

    std::vector<std::unique_ptr<GPUResource>> committedResources_;
    std::size_t nextHandle_ = 0;
};