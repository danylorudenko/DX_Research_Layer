#pragma once

#include <pch.hpp>

#include <Rendering\GPUFoundation.hpp>
#include <Rendering\Data\Resource\GPUResource.hpp>


class GPUResourceDirectID
{
public:
    GPUResourceDirectID();
    GPUResourceDirectID(std::size_t ID);

    GPUResourceDirectID(GPUResourceDirectID const& rhs);
    GPUResourceDirectID(GPUResourceDirectID&& rhs);

    GPUResourceDirectID& operator=(GPUResourceDirectID const& rhs);
    GPUResourceDirectID& operator=(GPUResourceDirectID&& rhs);

    std::size_t ID_ = 0;
};

//////////////////////////////////////////////////////////////////////////////////

class GPUResourceAllocator
{
public:
    GPUResourceAllocator();
    GPUResourceAllocator(GPUFoundation const& foundation);

    GPUResourceAllocator(GPUResourceAllocator const&) = delete;
    GPUResourceAllocator(GPUResourceAllocator&& rhs);

    GPUResourceAllocator& operator=(GPUResourceAllocator const&) = delete;
    GPUResourceAllocator& operator=(GPUResourceAllocator&& rhs);

    GPUResourceDirectID Alloc(D3D12_RESOURCE_DESC const& resourceDesc, D3D12_RESOURCE_STATES initialState);


private:
    GPUFoundation const* foundation_;

    std::vector<std::unique_ptr<GPUResource>> committedResources_;
};