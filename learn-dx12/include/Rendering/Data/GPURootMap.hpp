#pragma once

#include <pch.hpp>

#include <Rendering\Data\Resource\View\GPUResourceNativeView.hpp>

class GPURootMap
{
public:
    GPURootMap();
    GPURootMap(GPURootMap const& rhs);
    GPURootMap(GPURootMap&& rhs);

    GPURootMap& operator=(GPURootMap const& rhs);
    GPURootMap& operator=(GPURootMap&& rhs);

private:
    std::

    std::vector<GPUResourceView> rootResources_;
};