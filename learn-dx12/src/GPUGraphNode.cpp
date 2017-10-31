#include <Data\GPUGraphNode.hpp>

GPUGraphNode::GPUGraphNode() = default;

GPUGraphNode::GPUGraphNode(GRAPH_NODE_TYPE type, GPUEngine* engine, GPURootSignature* rootSignature, GPUPipelineState* pipelineState) :
    nodeType_(type),
    executionEngine_(engine),
    rootSignature_(rootSignature),
    pipelineState_(pipelineState)
{

}

GPUGraphNode::GPUGraphNode(GPUGraphNode&&) = default;

GPUGraphNode& GPUGraphNode::operator=(GPUGraphNode&&) = default;

void GPUGraphNode::ImportChildNode(GPUGraphNode* outputProxy)
{
    childNode_ = outputProxy;
}

void GPUGraphNode::Process()
{
    SetRootSignature();
    TransitionResources();

    switch (nodeType_) {
    case GPAPH_NODE_TYPE_GRAPHICS:
        // Regular rendering.

        break;
    case GRAPH_NODE_TYPE_COMPUTE:
        // Compute dispatching.

        break;
    }

    TriggerChildren();
}

void GPUGraphNode::TriggerChildren()
{
    childNode_->Process();
}

void GPUGraphNode::SetRootSignature()
{
    rootSignature_->SetRootSignature(executionEngine_);
    rootSignature_->SetPassRootSignatureDescriptorTables(executionEngine_, frameIndex_);
}

void GPUGraphNode::TransitionResources()
{
    rootSignature_->TransitionRootResources(executionEngine_, frameIndex_);
}

