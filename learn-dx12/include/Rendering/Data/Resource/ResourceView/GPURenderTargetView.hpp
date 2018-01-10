#pragma once

#include <Rendering\Data\Resource\ResourceView\GPUResourceView.hpp>

class GPURenderTargetView : public GPUResourceView
{
public:
    GPURenderTargetView();
    GPURenderTargetView(GPURenderTargetView const& rhs);
    GPURenderTargetView(GPURenderTargetView&& rhs);

    GPURenderTargetView& operator=(GPURenderTargetView const& rhs);
    GPURenderTargetView& operator=(GPURenderTargetView&& rhs);

    virtual void Bind(GPUEngine& executionEngine) = 0;

    virtual void Bind(GPUEngine& executionEngine, D3D12_CPU_DESCRIPTOR_HANDLE const* depthStencilViewHandle) = 0;

protected:

    D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilHandle(std::size_t frameIndex) const;
};