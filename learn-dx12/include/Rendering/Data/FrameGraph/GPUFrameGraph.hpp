#pragma once

#include <pch.hpp>

#include <Rendering\Data\FrameGraph\GPUGraphicsGraphNode.hpp>
#include <Rendering\Data\FrameGraph\GPUComputeGraphNode.hpp>
#include <Rendering\Data\FrameGraph\GPUPresentGraphNode.hpp>

class GPUFrameGraph
{
public:
    using GraphQueue = std::vector<GPUGraphNode*>;
    using GraphIterator = GraphQueue::iterator;
    
    GPUFrameGraph();
    GPUFrameGraph(GPUFrameGraph const&) = delete;
    GPUFrameGraph(GPUFrameGraph&& rhs);

    GPUFrameGraph& operator=(GPUFrameGraph const&) = delete;
    GPUFrameGraph& operator=(GPUFrameGraph&& rhs);

    GPUGraphicsGraphNode& AddGraphicsNode(GPUGraphNode* parent);
    GPUComputeGraphNode& AddComputeNode(GPUGraphNode* parent);
    GPUPresentGraphNode& AddPresentNode(GPUGraphNode* parent);

    void ParseGraphToQueue();

    GraphIterator GraphQueueStart();
    GraphIterator GraphQueueEnd();


private:
    void RecursiveNodeParserService(GPUGraphNode* node, std::set<GPUGraphNode*>& visitedNodes);

    std::vector<std::unique_ptr<GPUGraphNode>> nodes_;
    std::vector<GPUGraphNode*> parentNodes_;

    GraphQueue parsedGraphList_;
};