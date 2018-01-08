#pragma once

#include <pch.hpp>

#include <Rendering\Data\Resource\GPUResourceAllocator.hpp>
#include <Rendering\Data\Resource\GPUResourceFrameTable.hpp>

class GPUResourceFactory
{
public:
    GPUResourceFactory();
    GPUResourceFactory(GPUFoundation const& foundation, std::size_t framebufferCount);
    GPUResourceFactory(GPUResourceFactory const&) = delete;
    GPUResourceFactory(GPUResourceFactory&& rhs);

    GPUResourceFactory& operator=(GPUResourceFactory const&) = delete;
    GPUResourceFactory& operator=(GPUResourceFactory&& rhs);

    GPUResourceID AllocStaticResource(std::size_t framesCount, D3D12_RESOURCE_DESC const& desc, D3D12_RESOURCE_STATES initialState = D3D12_RESOURCE_STATE_COMMON);

private:
    GPUResourceAllocator staticResourcesAllocator_;
    GPUResourceFrameTable staticResourcesTable_;
};