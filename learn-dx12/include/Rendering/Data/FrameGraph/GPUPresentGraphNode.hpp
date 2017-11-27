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

    void ImportRenderTarget(GPUFrameResource* renderTarget);

    virtual void Process(UINT64 frameIndex) override;

protected:
    void TransitionRenderTargetState(int frameIndex);

    std::vector<GPUFrameResource*> renderTargets_;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain_;
};