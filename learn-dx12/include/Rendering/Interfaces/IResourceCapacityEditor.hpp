#pragma once

#include <pch.hpp>

#include <Rendering\Data\GPUResource.hpp>

class IResourceCapacityEditor
{
public:
    virtual std::size_t& GetResourceCapacityRef(GPUResource& resource) = 0;
};