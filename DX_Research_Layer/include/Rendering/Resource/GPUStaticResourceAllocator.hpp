#pragma once

#include <pch.hpp>

#include <Rendering\Resource\GPUResourceHandle.hpp>
#include <Rendering\Resource\GPUResource.hpp>

namespace DXRL
{

class GPUDelegate;

class GPUStaticResourceAllocator
{
public:
    GPUStaticResourceAllocator();
    GPUStaticResourceAllocator(GPUDelegate& foundation);

    GPUStaticResourceAllocator(GPUStaticResourceAllocator const&) = delete;
    GPUStaticResourceAllocator(GPUStaticResourceAllocator&& rhs);

    GPUStaticResourceAllocator& operator=(GPUStaticResourceAllocator const&) = delete;
    GPUStaticResourceAllocator& operator=(GPUStaticResourceAllocator&& rhs);

    GPUResourceHandle AllocDefault(D3D12_RESOURCE_DESC const& resourceDesc, D3D12_RESOURCE_STATES initialState);
    GPUResourceHandle AllocUpload(D3D12_RESOURCE_DESC const& resourceDesc, D3D12_RESOURCE_STATES initialState);
    GPUResourceHandle AllocReadback(D3D12_RESOURCE_DESC const& resourceDesc, D3D12_RESOURCE_STATES initialState);
    GPUResource& AccessResource(GPUResourceHandle const& id);


private:
    GPUDelegate* foundation_;

    std::vector<std::unique_ptr<GPUResource>> committedResources_;
};

}