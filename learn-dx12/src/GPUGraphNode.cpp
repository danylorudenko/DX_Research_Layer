#include <Rendering\Data\FrameGraph\GPUGraphNode.hpp>

GPUGraphNode::GPUGraphNode() = default;

GPUGraphNode::GPUGraphNode(GPUEngine& engine, GPURootSignature&& rootSignature, GPUPipelineState&& pipelineState) :
    executionEngine_(&engine),
    rootSignature_(std::move(rootSignature)),
    pipelineState_(std::move(pipelineState))
{

}

GPUGraphNode::GPUGraphNode(GPUGraphNode&&) = default;

GPUGraphNode& GPUGraphNode::operator=(GPUGraphNode&&) = default;

void GPUGraphNode::ImportChildNode(GPUGraphNode* outputProxy)
{
    childNodes_.push_back(outputProxy);
}

void GPUGraphNode::ImportRootSignature(GPURootSignature&& rootSignature)
{
    rootSignature_ = std::move(rootSignature);
}

void GPUGraphNode::ImportPipelineState(GPUPipelineState&& pipelineState)
{
    pipelineState_ = std::move(pipelineState);
}

GPUGraphNode& GPUGraphNode::GetChild(std::size_t childIndex)
{
    return *(childNodes_[childIndex]);
}

int GPUGraphNode::ChildCount() const
{
    return static_cast<int>(childNodes_.size());
}

void GPUGraphNode::BindPassRoot(std::size_t frameIndex)
{
    rootSignature_.BindPassRootSignature(executionEngine_);
    rootSignature_.BindPassDescriptorTables(executionEngine_, frameIndex);
}

void GPUGraphNode::TransitionPassResources(std::size_t frameIndex)
{
    rootSignature_.TransitionRootResources(executionEngine_, frameIndex);
}

