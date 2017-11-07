#pragma once

#include <pch.hpp>

#include <Data\FrameGraph\GPUGraphNode.hpp>
#include <Data\GPURenderItem.hpp>

class GPUGraphicsGraphNode : public GPUGraphNode
{
public:
    GPUGraphicsGraphNode();
    GPUGraphicsGraphNode(GPUEngine* engine, GPURootSignature* rootSignature, GPUPipelineState* pipelineState);
    GPUGraphicsGraphNode(GPUGraphicsGraphNode const&) = delete;
    GPUGraphicsGraphNode(GPUGraphicsGraphNode&& rhs);

    GPUGraphicsGraphNode& operator=(GPUGraphicsGraphNode const&) = delete;
    GPUGraphicsGraphNode& operator=(GPUGraphicsGraphNode&& rhs);

    virtual void Process() override;

private:
    void IterateRenderItems();

    std::vector<GPURenderItem> renderItems_;
    
};