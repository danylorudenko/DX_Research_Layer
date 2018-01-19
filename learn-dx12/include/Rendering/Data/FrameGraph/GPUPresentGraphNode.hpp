#pragma once

#include <pch.hpp>

#include <Rendering\Data\FrameGraph\GPUGraphNode.hpp>

class GPUPresentGraphNode : public GPUGraphNode
{
public:
    GPUPresentGraphNode();
    GPUPresentGraphNode(Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain, GPUEngine* executionEngine);

    GPUPresentGraphNode(GPUPresentGraphNode const&) = delete;
    GPUPresentGraphNode(GPUPresentGraphNode&& rhs);

    GPUPresentGraphNode& operator=(GPUPresentGraphNode const&) = delete;
    GPUPresentGraphNode& operator=(GPUPresentGraphNode&& rhs);

    void ImportRenderTarget(GPUResourceViewHandle* renderTarget);

    virtual void Process(std::size_t frameIndex) override;

protected:
    void TransitionRenderTargetState(std::size_t frameIndex);

    std::vector<GPUResourceViewHandle> renderTargets_;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain_;
};