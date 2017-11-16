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

    auto const parentsCount = parentNodes_.size();
    for (size_t i = 0; i < parentsCount; i++) {
        RecursiveNodeParserService(parentNodes_[i], visitedNodes);
    }

    // TODO
    // NOT COMPLETE
}

void GPUFrameGraph::RecursiveNodeParserService(GPUGraphNode* node, std::set<GPUGraphNode*>& visitedNodes)
{
    auto const nodeChildCount = node->ChildCount();
    for (size_t i = 0; i < nodeChildCount; i++) {
        RecursiveNodeParserService(node->GetChild(i), visitedNodes);
    }


}