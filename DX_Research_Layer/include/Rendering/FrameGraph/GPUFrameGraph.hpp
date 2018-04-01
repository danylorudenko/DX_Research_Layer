#pragma once

#include <pch.hpp>

#include <Rendering\FrameGraph\GPUGraphicsGraphNode.hpp>
#include <Rendering\FrameGraph\GPUComputeGraphNode.hpp>
#include <Rendering\FrameGraph\GPUPresentGraphNode.hpp>

namespace DXRL
{

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

    GPUGraphicsGraphNode& AddGraphicsNode(GPUGraphNode* parent, GPUEngine& executionEngine, GPUPipelineState&& pipelineState, GPURootSignature&& rootSignature);
    GPUComputeGraphNode& AddComputeNode(GPUGraphNode* parent, GPUEngine& executionEngine);
    GPUPresentGraphNode& AddPresentNode(GPUGraphNode& parent, GPUSwapChain& swapChain, GPUEngine& executionEngine);

    void ParseGraphToQueue();

    GraphIterator GraphQueueStart();
    GraphIterator GraphQueueEnd();


private:
    void RecursiveNodeParserService(GPUGraphNode* node, std::set<GPUGraphNode*>& visitedNodes);
    void InsertNodeToHierarchy(GPUGraphNode* parent, GPUGraphNode* node);

    std::vector<std::unique_ptr<GPUGraphNode>> nodes_;
    std::vector<GPUGraphNode*> parentNodes_;

    GraphQueue parsedGraphList_;
};

}