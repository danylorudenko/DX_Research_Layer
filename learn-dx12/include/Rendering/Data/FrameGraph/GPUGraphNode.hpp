#pragma once

#include <pch.hpp>

#include <Rendering\GPUEngine.hpp>
#include <Rendering\Data\GPURootSignature.hpp>
#include <Rendering\Data\GPUPipelineState.hpp>

class GPUGraphNode
{
public:
    GPUGraphNode();
    GPUGraphNode(GPUEngine& engine, GPURootSignature&& rootSignature, GPUPipelineState&& pipelineState);
    GPUGraphNode(GPUGraphNode const&) = delete;
    GPUGraphNode(GPUGraphNode&& rhs);

    GPUGraphNode& operator=(GPUGraphNode const&) = delete;
    GPUGraphNode& operator=(GPUGraphNode&& rhs);

    void ImportRootSignature(GPURootSignature&& rootSignature);
    void ImportPipelineState(GPUPipelineState&& pipelineState);

    void ImportChildNode(GPUGraphNode* outputNode);

    GPUGraphNode& GetChild(std::size_t childIndex);
    int ChildCount() const;

    virtual void Process(std::size_t frameIndex) = 0;

protected:
    void BindPassRoot(std::size_t frameIndex);
    void TransitionPassResources(std::size_t frameIndex);

    GPUEngine* executionEngine_ = nullptr;

    GPURootSignature rootSignature_;
    GPUPipelineState pipelineState_;

    std::vector<GPUGraphNode*> childNodes_;
};