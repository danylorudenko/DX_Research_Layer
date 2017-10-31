#include <Data\GPUGraphNode.hpp>

GPUGraphNode::GPUGraphNode() = default;

GPUGraphNode::GPUGraphNode(GPUEngine* engine, GPURootSignature* rootSignature, GPUPipelineState* pipelineState) :
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
    SubmitProcessing();

    TriggerChildren();
}

void GPUGraphNode::TriggerChildren()
{
    childNode_->Process();
}

void GPUGraphNode::SetRootSignature()
{
    rootSignature_->SetRootSignature(executionEngine_);
    rootSignature_->SetRootSignatureDescriptorTables(executionEngine_, frameIndex_);
}

void GPUGraphNode::TransitionResources()
{
    rootSignature_->TransitionRootResources(executionEngine_, frameIndex_);
}

