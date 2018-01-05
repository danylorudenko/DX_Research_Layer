#pragma once

#include <pch.hpp>

#include <Rendering\Data\Resource\GPUResourceFrameTable.hpp>


class GPUResourceView
{
public:
    GPUResourceView();
    GPUResourceView(GPUResourceView const& rhs);
    GPUResourceView(GPUResourceView&& rhs);

    GPUResourceView& operator=(GPUResourceView const& rhs);
    GPUResourceView& operator=(GPUResourceView&& rhs);

protected:
    std::size_t offsetInHeap_;
    GPUResourceID describedResource_ = SIZE_MAX;
};