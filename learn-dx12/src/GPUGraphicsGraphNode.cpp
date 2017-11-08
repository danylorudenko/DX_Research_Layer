#include <Data\FrameGraph\GPUGraphicsGraphNode.hpp>

GPUGraphicsGraphNode::GPUGraphicsGraphNode() = default;

GPUGraphicsGraphNode::GPUGraphicsGraphNode(GPUEngine* engine, GPURootSignature* rootSignature, GPUPipelineState* pipelineState) :
    GPUGraphNode(engine, rootSignature, pipelineState)
{ }

GPUGraphicsGraphNode::GPUGraphicsGraphNode(GPUGraphicsGraphNode&& rhs) :
    GPUGraphNode(std::move(rhs))
{ }

GPUGraphicsGraphNode& GPUGraphicsGraphNode::operator=(GPUGraphicsGraphNode&& rhs)
{
    GPUGraphNode::operator=(std::move(rhs));
    return *this;
}

void GPUGraphicsGraphNode::Process()
{
    SetPassRootSignature();
    TransitionPassResources();

    IterateRenderItems();
}

void GPUGraphicsGraphNode::BindRenderItemRootResources(GPURenderItem& item)
{
    auto const resCount = item.perItemResourceDescriptors_.size();
    for (size_t i = 0; i < resCount; i++) {

    }
    
}