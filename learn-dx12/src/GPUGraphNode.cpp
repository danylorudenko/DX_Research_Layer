#include <Rendering\Data\FrameGraph\GPUGraphNode.hpp>

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
    childNodes_.push_back(outputProxy);
}

void GPUGraphNode::ImportRootSignature(GPURootSignature* rootSignature)
{
    rootSignature_ = rootSignature;
}

void GPUGraphNode::ImportPipelineState(GPUPipelineState* pipelineState)
{
    pipelineState_ = pipelineState;
}

GPUGraphNode* GPUGraphNode::GetChild(int childIndex)
{
    return childNodes_[childIndex];
}

int GPUGraphNode::ChildCount() const
{
    return static_cast<int>(childNodes_.size());
}

bool GPUGraphNode::SynchronizeFrames(UINT64 frameIndex) {
    if (frameIndex < lastFrameIndex_) {
        lastFrameIndex_ = frameIndex;
        return true;
    }
    
    return false;
}

void GPUGraphNode::SetPassRootSignature()
{
    rootSignature_->SetPassRootSignature(executionEngine_);
    rootSignature_->SetPassRootSignatureDescriptorTables(executionEngine_, static_cast<UINT>(lastFrameIndex_));
}

void GPUGraphNode::TransitionPassResources()
{
    rootSignature_->TransitionRootResources(executionEngine_, static_cast<UINT>(lastFrameIndex_));
}

