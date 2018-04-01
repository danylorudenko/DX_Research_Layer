#pragma once

#include <pch.hpp>

#include <Rendering\Resource\GPUResourceHandle.hpp>

namespace DXRL
{

class GPUResourceViewContextTable
{
public:
    GPUResourceViewContextTable();
    GPUResourceViewContextTable(std::size_t framesCount);

    GPUResourceViewContextTable(GPUResourceViewContextTable const&) = delete;
    GPUResourceViewContextTable(GPUResourceViewContextTable&& rhs);

    GPUResourceViewContextTable& operator=(GPUResourceViewContextTable const&) = delete;
    GPUResourceViewContextTable& operator=(GPUResourceViewContextTable&& rhs);

    GPUResourceViewHandle InsertView(std::size_t frameCount, GPUResourceViewDirectHandle const* resourceIDs, GPUResourceViewAllocator& viewAllocator);

    GPUResourceViewDirectHandle FetchDirectHandle(std::size_t frameIndex, GPUResourceViewHandle const& virtualHandle);

private:
    using FrameContext = std::vector<GPUResourceViewDirectHandle>;

    std::vector<FrameContext> frameList_;
    std::size_t virtualHandleCounter_ = 0;
};

}