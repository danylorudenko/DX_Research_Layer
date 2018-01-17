#pragma once

#include <pch.hpp>

#include <Rendering\Data\Resource\GPUResourceHandle.hpp>

class GPUResourceViewTable
{
public:
    GPUResourceViewTable();
    GPUResourceViewTable(std::size_t framesCount);

    GPUResourceViewTable(GPUResourceViewTable const&) = delete;
    GPUResourceViewTable(GPUResourceViewTable&& rhs);

    GPUResourceViewTable& operator=(GPUResourceViewTable const&) = delete;
    GPUResourceViewTable& operator=(GPUResourceViewTable&& rhs);

    GPUResourceViewHandle InsertView(std::size_t frameCount, GPUResourceViewDirectHandle const* resourceIDs, GPUResourceViewAllocator& viewAllocator);

    GPUResourceViewDirectHandle FetchDirectHandle(std::size_t frameIndex, GPUResourceViewHandle const& virtualHandle);

private:
    using FrameContext = std::vector<GPUResourceViewDirectHandle>;

    std::vector<FrameContext> frameList_;
    
};