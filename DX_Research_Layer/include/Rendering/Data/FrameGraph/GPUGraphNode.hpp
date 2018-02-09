#pragma once

#include <pch.hpp>

#include <Rendering\GPUEngine.hpp>
#include <Rendering\Data\GPURootSignature.hpp>
#include <Rendering\Data\GPUPipelineState.hpp>

namespace DXRL
{

class GPUGraphNode
{
public:
    GPUGraphNode();
    GPUGraphNode(GPUEngine& engine);
    GPUGraphNode(GPUGraphNode const&) = delete;
    GPUGraphNode(GPUGraphNode&& rhs);

    GPUGraphNode& operator=(GPUGraphNode const&) = delete;
    GPUGraphNode& operator=(GPUGraphNode&& rhs);

    void ImportChildNode(GPUGraphNode* outputNode);

    GPUGraphNode* GetChild(std::size_t childIndex);
    int ChildCount() const;

    virtual void Process(std::size_t frameIndex) = 0;

protected:
    GPUEngine* executionEngine_ = nullptr;

    std::vector<GPUGraphNode*> childNodes_;
};

}