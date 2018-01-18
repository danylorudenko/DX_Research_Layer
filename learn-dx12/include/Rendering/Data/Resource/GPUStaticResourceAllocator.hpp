#pragma once

#include <pch.hpp>

#include <Rendering\Data\Resource\GPUResourceHandle.hpp>
#include <Rendering\Data\Resource\GPUResource.hpp>

class GPUFoundation;

class GPUStaticResourceAllocator
{
public:
    GPUStaticResourceAllocator();
    GPUStaticResourceAllocator(GPUFoundation& foundation);

    GPUStaticResourceAllocator(GPUStaticResourceAllocator const&) = delete;
    GPUStaticResourceAllocator(GPUStaticResourceAllocator&& rhs);

    GPUStaticResourceAllocator& operator=(GPUStaticResourceAllocator const&) = delete;
    GPUStaticResourceAllocator& operator=(GPUStaticResourceAllocator&& rhs);

    GPUResourceHandle AllocDefault(D3D12_RESOURCE_DESC const& resourceDesc, D3D12_RESOURCE_STATES initialState);
    GPUResourceHandle AllocUpload(D3D12_RESOURCE_DESC const& resourceDesc, D3D12_RESOURCE_STATES initialState);
    GPUResourceHandle AllocReadback(D3D12_RESOURCE_DESC const& resourceDesc, D3D12_RESOURCE_STATES initialState);
    GPUResource& AccessResource(GPUResourceHandle const& id);


private:
    GPUFoundation* foundation_;

    std::vector<std::unique_ptr<GPUResource>> committedResources_;
};