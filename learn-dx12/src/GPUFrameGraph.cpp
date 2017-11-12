#include <Rendering\Data\FrameGraph\GPUFrameGraph.hpp>

GPUFrameGraph::GPUFrameGraph() = default;

GPUFrameGraph::GPUFrameGraph(GPUFrameGraph const& rhs) = default;

GPUFrameGraph::GPUFrameGraph(GPUFrameGraph&& rhs) = default;

GPUFrameGraph& GPUFrameGraph::operator=(GPUFrameGraph&& rhs) = default;

void GPUFrameGraph::AddParentNode(GPUGraphNode* parentNode)
{
    parentNodes_.push_back(parentNode);
}

void GPUFrameGraph::ParseGraphToQueue()
{
    std::set<GPUGraphNode*> visitedNodes;
}