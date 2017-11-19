#include <Rendering\Data\FrameGraph\GPUFrameGraph.hpp>

GPUFrameGraph::GPUFrameGraph() = default;

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
    std::set<GPUGraphNode*> visitedNodes{};

    auto const parentsCount = parentNodes_.size();
    for (size_t i = 0; i < parentsCount; i++) {
        RecursiveNodeParserService(parentNodes_[i], visitedNodes);
    }

    // Reversing queue to simulate the stack behaviour and iterate from start to end.
    std::reverse(parsedGraphList_.begin(), parsedGraphList_.end());
}

GPUFrameGraph::GraphIterator GPUFrameGraph::GraphQueueStart()
{
    return parsedGraphList_.begin();
}

GPUFrameGraph::GraphIterator GPUFrameGraph::GraphQueueEnd()
{
    return parsedGraphList_.end();
}

void GPUFrameGraph::RecursiveNodeParserService(GPUGraphNode* node, std::set<GPUGraphNode*>& visitedNodes)
{
    visitedNodes.emplace(node);

    int nonVisitedChildren = 0;
    auto const nodeChildCount = node->ChildCount();
    for (int i = 0; i < nodeChildCount; i++) {
        auto nodeIterator = visitedNodes.find(node);
        // If node is not found == not visited.
        if (nodeIterator != visitedNodes.end()) {
            RecursiveNodeParserService(node->GetChild(i), visitedNodes);
            nonVisitedChildren++;
        }
    }

    // so the node is the dead end.
    if (nonVisitedChildren > 0) {
        parsedGraphList_.push_back(node);
    }
}