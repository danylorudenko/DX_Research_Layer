#pragma once

#include <Rendering\Data\Resource\ResourceView\GPURenderTargetView.hpp>

class GPUSwapChainRenderTargetView : public GPURenderTargetView
{
    GPUSwapChainRenderTargetView();
    GPUSwapChainRenderTargetView(GPUSwapChainRenderTargetView const& rhs);
    GPUSwapChainRenderTargetView(GPUSwapChainRenderTargetView&& rhs);

    GPUSwapChainRenderTargetView& operator=(GPUSwapChainRenderTargetView const& rhs);
    GPUSwapChainRenderTargetView& operator=(GPUSwapChainRenderTargetView&& rhs);

    virtual void Bind(GPUEngine& executionEngine) override;

};