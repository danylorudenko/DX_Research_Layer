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
    // Clear obsolete parsed graph
    parsedGraphList_.clear();
    
    std::set<GPUGraphNode*> visitedNodes;
    auto const parentNodesCount = parentNodes_.size();
    for (size_t i = 0; i < parentNodesCount; i++) {
        visitedNodes.insert(parentNodes_[i]);
        parsedGraphList_.push_back(parentNodes_[i]);
    }

    // TODO
    // NOT COMPLETE
}