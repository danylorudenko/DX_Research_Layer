#include <Rendering\Data\FrameGraph\GPUGraphNode.hpp>

GPUGraphNode::GPUGraphNode() = default;

GPUGraphNode::GPUGraphNode(GPUEngine* engine, GPURootSignature* rootSignature, GPUPipelineState* pipelineState, int frameBufferCount) :
    executionEngine_(engine),
    rootSignature_(rootSignature),
    pipelineState_(pipelineState),
    frameBufferCount_(frameBufferCount)
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

void GPUGraphNode::BindPassRoot(int frameIndex)
{
    rootSignature_->BindPassRootSignature(executionEngine_);
    rootSignature_->BindPassRootSignatureDescriptorTables(executionEngine_, frameIndex);
}

void GPUGraphNode::TransitionPassResources(int frameIndex)
{
    rootSignature_->TransitionRootResources(executionEngine_, frameIndex);
}

