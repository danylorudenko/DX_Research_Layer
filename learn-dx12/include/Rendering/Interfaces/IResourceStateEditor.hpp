#pragma once

#include <pch.hpp>

#include <Rendering\Data\GPUResource.hpp>

class IResourceStateEditor
{
public:
    virtual D3D12_RESOURCE_STATES& GetResourceStateRef(GPUResource& resource) = 0;
};