#pragma once

#include <pch.hpp>

#include <Rendering\GPUEngine.hpp>
#include <Data\GPURootSignature.hpp>
#include <Data\GPUPipelineState.hpp>

class GPUGraphNode
{
public:
    GPUGraphNode();
    GPUGraphNode(GPUEngine* engine, GPURootSignature* rootSignature, GPUPipelineState* pipelineState);
    GPUGraphNode(GPUGraphNode const&) = delete;
    GPUGraphNode(GPUGraphNode&& rhs);

    GPUGraphNode& operator=(GPUGraphNode const&) = delete;
    GPUGraphNode& operator=(GPUGraphNode&& rhs);

    void ImportRootSignature(GPURootSignature* rootSignature);
    void ImportPipelineState(GPUPipelineState* pipelineState);

    // For now, only one child is available.
    void ImportChildNode(GPUGraphNode* outputNode);

    virtual void Process(UINT64 frameIndex) = 0;

protected:
    bool SynchronizeFrames(UINT64 frameIndex);

    void SetPassRootSignature();
    void TransitionPassResources();

    GPUEngine* executionEngine_ = nullptr;

    GPURootSignature* rootSignature_ = nullptr;
    GPUPipelineState* pipelineState_ = nullptr;

    GPUGraphNode* childNode_ = nullptr;

    UINT64 lastFrameIndex_ = 0ULL;
};