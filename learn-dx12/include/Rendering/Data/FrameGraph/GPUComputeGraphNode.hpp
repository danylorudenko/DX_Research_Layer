#pragma once

#include <pch.hpp>

#include <Rendering\Data\FrameGraph\GPUGraphNode.hpp>

class GPUComputeGraphNode : public GPUGraphNode
{
public:
    GPUComputeGraphNode();
    GPUComputeGraphNode(GPUComputeGraphNode const&) = delete;
    GPUComputeGraphNode(GPUComputeGraphNode&& rhs);

    GPUComputeGraphNode& operator=(GPUComputeGraphNode const&) = delete;
    GPUComputeGraphNode& operator=(GPUComputeGraphNode&& rhs);

    
};