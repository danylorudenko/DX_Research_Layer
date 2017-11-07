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

    virtual void Process() = 0;

private:
    void SetRootSignature();
    void TransitionResources();

    GPUEngine* executionEngine_ = nullptr;

    GPURootSignature* rootSignature_ = nullptr;
    GPUPipelineState* pipelineState_ = nullptr;

    GPUGraphNode* childNode_ = nullptr;

    UINT64 frameIndex_ = 0ULL;
};