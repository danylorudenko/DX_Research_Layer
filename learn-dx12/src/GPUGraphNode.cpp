#include <Data\FrameGraph\GPUGraphNode.hpp>

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

void GPUGraphNode::ImportRootSignature(GPURootSignature* rootSignature)
{
    rootSignature_ = rootSignature;
}

void GPUGraphNode::ImportPipelineState(GPUPipelineState* pipelineState)
{
    pipelineState_ = pipelineState;
}

void GPUGraphNode::SetPassRootSignature()
{
    rootSignature_->SetPassRootSignature(executionEngine_);
    rootSignature_->SetPassRootSignatureDescriptorTables(executionEngine_, frameIndex_);
}

void GPUGraphNode::TransitionPassResources()
{
    rootSignature_->TransitionRootResources(executionEngine_, frameIndex_);
}

