#include <Rendering\FrameGraph\GPUGraphNode.hpp>

namespace DXRL
{

GPUGraphNode::GPUGraphNode() = default;

GPUGraphNode::GPUGraphNode(GPUEngine& engine) :
    executionEngine_(&engine)
{

}

GPUGraphNode::GPUGraphNode(GPUGraphNode&&) = default;

GPUGraphNode& GPUGraphNode::operator=(GPUGraphNode&&) = default;

void GPUGraphNode::ImportChildNode(GPUGraphNode* outputProxy)
{
    childNodes_.push_back(outputProxy);
}

GPUGraphNode* GPUGraphNode::GetChild(std::size_t childIndex)
{
    return childNodes_[childIndex];
}

int GPUGraphNode::ChildCount() const
{
    return static_cast<int>(childNodes_.size());
}

}