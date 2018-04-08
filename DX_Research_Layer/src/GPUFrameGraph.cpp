#include <pch.hpp>

#include <Rendering\FrameGraph\GPUFrameGraph.hpp>

namespace DXRL
{

GPUFrameGraph::GPUFrameGraph() = default;

GPUFrameGraph::GPUFrameGraph(GPUFrameGraph&& rhs) = default;

GPUFrameGraph& GPUFrameGraph::operator=(GPUFrameGraph&& rhs) = default;

GPUGraphicsGraphNode& GPUFrameGraph::AddGraphicsNode(GPUGraphNode* parent, GPUEngine& executionEngine, GPUPipelineState&& pipelineState, GPURootSignature&& rootSignature)
{
    auto* newNode = new GPUGraphicsGraphNode{ executionEngine, std::move(pipelineState), std::move(rootSignature) };
    nodes_.emplace_back(std::unique_ptr<GPUGraphicsGraphNode>{ newNode });
    InsertNodeToHierarchy(parent, newNode);
    return *newNode;
}

GPUComputeGraphNode& GPUFrameGraph::AddComputeNode(GPUGraphNode* parent, GPUEngine& executionEngine)
{
    return GPUComputeGraphNode{};
}

GPUPresentGraphNode& GPUFrameGraph::AddPresentNode(GPUGraphNode& parent, GPUSwapChain& swapChain, GPUEngine& executionEngine)
{
    auto* newNode = new GPUPresentGraphNode{ swapChain, executionEngine };
    nodes_.emplace_back(std::unique_ptr<GPUPresentGraphNode>{ newNode });
    InsertNodeToHierarchy(&parent, newNode);
    return *newNode;
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
    int nonVisitedChildren = 0;
    auto const nodeChildCount = node->ChildCount();
    for (int i = 0; i < nodeChildCount; i++) {
        auto nodeIterator = visitedNodes.find(node);
        // If node is not found == not visited.
        if (nodeIterator == visitedNodes.end()) {
            RecursiveNodeParserService(node->GetChild(i), visitedNodes);
            nonVisitedChildren++;
            visitedNodes.emplace(node);
        }
    }

    // so the node is the dead end.
    if (nonVisitedChildren > 0 || nodeChildCount == 0) {
        parsedGraphList_.push_back(node);
    }
}

void GPUFrameGraph::InsertNodeToHierarchy(GPUGraphNode* parent, GPUGraphNode* node)
{
    if (parent != nullptr) {
        parent->ImportChildNode(node);
    }
    else {
        parentNodes_.push_back(node);
    }
}

}