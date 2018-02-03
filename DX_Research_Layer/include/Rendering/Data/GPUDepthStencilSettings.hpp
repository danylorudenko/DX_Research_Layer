#pragma once

#include <pch.hpp>

namespace DXRL
{

struct GPUDepthStencilSettings
{
    void ActivateDepth() { depthStencilActiveFlags_ = depthStencilActiveFlags_ | 1; }
    void ActivateStencil() { depthStencilActiveFlags_ = depthStencilActiveFlags_ | (1 << 1); }

    void DisableDepth() { depthStencilActiveFlags_ = depthStencilActiveFlags_ & ~1; }
    void DisableStencil() { depthStencilActiveFlags_ = depthStencilActiveFlags_ & ~(1 << 1); }

    bool DepthActive() const { return (depthStencilActiveFlags_) & 1; }
    bool StencilActive() const { return (depthStencilActiveFlags_ << 1) & 1; }

    std::uint8_t depthStencilActiveFlags_ = 0;
    D3D12_CLEAR_FLAGS depthStencilClearFlags_ = (D3D12_CLEAR_FLAGS)0;
};

}