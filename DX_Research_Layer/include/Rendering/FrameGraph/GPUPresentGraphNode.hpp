#pragma once

#include <pch.hpp>

#include <Rendering\FrameGraph\GPUGraphNode.hpp>
#include <Rendering\GPUEngine\GPUSwapChain.hpp>

namespace DXRL
{

class GPUPresentGraphNode : public GPUGraphNode
{
public:
    GPUPresentGraphNode();
    GPUPresentGraphNode(GPUSwapChain& swapChain, GPUEngine& executionEngine);

    GPUPresentGraphNode(GPUPresentGraphNode const&) = delete;
    GPUPresentGraphNode(GPUPresentGraphNode&& rhs);

    GPUPresentGraphNode& operator=(GPUPresentGraphNode const&) = delete;
    GPUPresentGraphNode& operator=(GPUPresentGraphNode&& rhs);

    void ImportRenderTarget(GPUResourceViewHandle const& renderTarget);

    virtual void Process(std::size_t frameIndex) override;

protected:
    void TransitionRenderTargetState(std::size_t frameIndex);

    GPUResourceViewHandle renderTarget_;
    GPUSwapChain* swapChain_;
};

}