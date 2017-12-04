#pragma once

#include <pch.hpp>

#include <Rendering\Data\GPUResource.hpp>

class IResourceVirtualGPUAddressGetter
{
public:
    virtual D3D12_GPU_VIRTUAL_ADDRESS GetResourceGPUVirtualAddress(GPUResource& resource) = 0;
};