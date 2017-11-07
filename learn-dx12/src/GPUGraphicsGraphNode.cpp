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

    // LALALALAL
}