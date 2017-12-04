#pragma once

#include <pch.hpp>

#include <Rendering\Data\GPUResource.hpp>

class IResourceCapacityGetter
{
public:
    virtual std::size_t GetResourceCapacity(GPUResource const& resource) = 0;
};