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

void GPUGraphNode::AddChild(GPUGraphResourceProxy* child)
{
    children_.push_back(child);
}

void GPUGraphNode::ClearChildren()
{
    children_.clear();
}

void GPUGraphNode::SetRootParams()
{
    //executionEngine_->Commit().SetGraphicsRootSignature(rootSignature_->Get());

}