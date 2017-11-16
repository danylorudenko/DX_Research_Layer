#pragma once

#include <pch.hpp>

#include <Rendering\Data\FrameGraph\GPUGraphNode.hpp>

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

    void AddParentNode(GPUGraphNode* parentNode);

    void ParseGraphToQueue();

    GraphIterator GraphQueueStart();
    GraphIterator GraphQueueEnd();


private:
    void RecursiveNodeParserService(GPUGraphNode* node, std::set<GPUGraphNode*>& visitedNodes);

    std::vector<GPUGraphNode*> parentNodes_;
    std::vector<GPUGraphNode*> parsedGraphList_;
};