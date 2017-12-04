#pragma once

#include <pch.hpp>

#include <Rendering\Data\GPUResource.hpp>

class IResourceSizeEditor
{
public:
    virtual std::size_t& GetResourceSizeRef(GPUResource& resource) = 0;
};