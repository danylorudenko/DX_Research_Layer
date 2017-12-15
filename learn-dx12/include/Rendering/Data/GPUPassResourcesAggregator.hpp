#pragma once

#include <pch.hpp>

#include <Rendering\Data\Resource\View\GPUResourceNativeView.hpp>

class GPUPassResourcesAggregator
{
public:
    GPUPassResourcesAggregator();
    GPUPassResourcesAggregator(GPUPassResourcesAggregator const& rhs);
    GPUPassResourcesAggregator(GPUPassResourcesAggregator&& rhs);

    GPUPassResourcesAggregator& operator=(GPUPassResourcesAggregator const& rhs);
    GPUPassResourcesAggregator& operator=(GPUPassResourcesAggregator&& rhs);

private:
    // GPURootResourceMap

    std::vector<GPUResourceNativeView> renderTargets_;
    
};