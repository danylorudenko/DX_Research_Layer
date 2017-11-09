#pragma once

#include <pch.hpp>

#include <Data\FrameGraph\GPUGraphNode.hpp>

class GPUPresentGraphNode : public GPUGraphNode
{
public:
    GPUPresentGraphNode();
    GPUPresentGraphNode(Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain);

    GPUPresentGraphNode(GPUPresentGraphNode const&) = delete;
    GPUPresentGraphNode(GPUPresentGraphNode&& rhs);

    GPUPresentGraphNode& operator=(GPUPresentGraphNode const&) = delete;
    GPUPresentGraphNode& operator=(GPUPresentGraphNode&& rhs);

    virtual void Process(UINT64 frameIndex) override;

protected:
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain_;
};