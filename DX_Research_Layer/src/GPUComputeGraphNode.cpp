#include <Rendering\Data\FrameGraph\GPUComputeGraphNode.hpp>

namespace DXRL
{

GPUComputeGraphNode::GPUComputeGraphNode() = default;

GPUComputeGraphNode::GPUComputeGraphNode(GPUComputeGraphNode&& rhs) = default;

GPUComputeGraphNode& GPUComputeGraphNode::operator=(GPUComputeGraphNode&& rhs) = default;

void GPUComputeGraphNode::Process(std::size_t frameIndex)
{

}

}