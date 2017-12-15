#pragma once

#include <pch.hpp>

#include <Rendering\Data\Resource\GPUResource.hpp>

class GPUResourceView
{
public:
    GPUResourceView();
    GPUResourceView(D3D12_RESOURCE_STATES targetState, GPUResource* describedResource);

    GPUResourceView(GPUResourceView const& rhs);
    GPUResourceView(GPUResourceView&& rhs);

    GPUResourceView& operator=(GPUResourceView const& rhs);
    GPUResourceView& operator=(GPUResourceView&& rhs);

    D3D12_RESOURCE_STATES ViewState() const;
    GPUResource const* Resource() const;

    virtual bool IsNativeView() const;

private:
    D3D12_RESOURCE_STATES targetState_;
    GPUResource* describedResource_;
};