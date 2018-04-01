#pragma once

#include <pch.hpp>

#include <Rendering\FrameGraph\GPUGraphNode.hpp>

namespace DXRL
{

class GPUComputeGraphNode : public GPUGraphNode
{
public:
    GPUComputeGraphNode();
    GPUComputeGraphNode(GPUComputeGraphNode const&) = delete;
    GPUComputeGraphNode(GPUComputeGraphNode&& rhs);

    GPUComputeGraphNode& operator=(GPUComputeGraphNode const&) = delete;
    GPUComputeGraphNode& operator=(GPUComputeGraphNode&& rhs);

    virtual void Process(std::size_t frameIndex) override;

private:

    GPURootSignature rootSignature_;
    GPUPipelineState pipelineState_;
};

}