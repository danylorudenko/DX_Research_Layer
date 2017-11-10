#pragma once

#include <pch.hpp>

#include <Rendering\Data\FrameGraph\GPUGraphNode.hpp>

class GPUFrameGraph
{
public:
    GPUFrameGraph();

    GPUFrameGraph(GPUFrameGraph const&) = delete;
    GPUFrameGraph(GPUFrameGraph&& rhs);

    GPUFrameGraph& operator=(GPUFrameGraph const&) = delete;
    GPUFrameGraph& operator=(GPUFrameGraph&& rhs);

    void AddParentNode(GPUGraphNode* parentNode);

private:
    void SynchronizeFrames();
    void CommitGraph();
    void FinishFrame();

    void ParseGraphToQueue();

    std::vector<GPUGraphNode*> parentNodes_;
    UINT64 frameIndex_ = 0ULL;
    std::vector<GPUGraphNode*> parsedGraphList_;
};