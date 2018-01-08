#pragma once

#include <pch.hpp>

#include <Rendering\GPUFoundation.hpp>
#include <Rendering\Data\Resource\GPUResource.hpp>
#include <Rendering\Data\Resource\GPUResourceID.hpp>

class GPUStaticResourceAllocator
{
public:
    GPUStaticResourceAllocator();
    GPUStaticResourceAllocator(GPUFoundation const& foundation);

    GPUStaticResourceAllocator(GPUStaticResourceAllocator const&) = delete;
    GPUStaticResourceAllocator(GPUStaticResourceAllocator&& rhs);

    GPUStaticResourceAllocator& operator=(GPUStaticResourceAllocator const&) = delete;
    GPUStaticResourceAllocator& operator=(GPUStaticResourceAllocator&& rhs);

    GPUResourceDirectID Alloc(D3D12_RESOURCE_DESC const& resourceDesc, D3D12_RESOURCE_STATES initialState);


private:
    GPUFoundation const* foundation_;

    std::vector<std::unique_ptr<GPUResource>> committedResources_;
};