#pragma once

#include <pch.hpp>

#include <Data\FrameGraph\GPUGraphNode.hpp>

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
    void StartFrame();
    void EndFrame();

    std::vector<GPUGraphNode*> parentNodes_;
    UINT64 frameIndex_ = 0ULL;
};