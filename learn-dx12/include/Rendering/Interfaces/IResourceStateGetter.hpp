#pragma once

#include <pch.hpp>

#include <Rendering\Data\GPUResource.hpp>

class IResourceStateGetter
{
public:
    virtual D3D12_RESOURCE_STATES& GetResourceState(GPUResource& resource) = 0;
};