#pragma once

#include <pch.hpp>

#include <Rendering\Data\GPURootMap.hpp>

class GPUPassResourcesAggregator
{
public:
    GPUPassResourcesAggregator();
    GPUPassResourcesAggregator(GPUPassResourcesAggregator const& rhs);
    GPUPassResourcesAggregator(GPUPassResourcesAggregator&& rhs);

    GPUPassResourcesAggregator& operator=(GPUPassResourcesAggregator const& rhs);
    GPUPassResourcesAggregator& operator=(GPUPassResourcesAggregator&& rhs);

private:
    GPURootMap rootMap_;

    std::vector<GPUResourceNativeView> renderTargets_;
    
};