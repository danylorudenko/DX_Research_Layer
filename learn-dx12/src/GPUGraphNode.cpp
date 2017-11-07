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

void GPUGraphNode::SetRootSignature()
{
    rootSignature_->SetRootSignature(executionEngine_);
    rootSignature_->SetPassRootSignatureDescriptorTables(executionEngine_, frameIndex_);
}

void GPUGraphNode::TransitionResources()
{
    rootSignature_->TransitionRootResources(executionEngine_, frameIndex_);
}

