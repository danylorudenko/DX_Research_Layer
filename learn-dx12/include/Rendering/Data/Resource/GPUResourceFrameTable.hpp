#pragma once

#include <pch.hpp>

#include <Rendering\Data\Resource\GPUResourceID.hpp>

class GPUResourceFrameTable
{
public:
    GPUResourceFrameTable();
    GPUResourceFrameTable(std::size_t framesCount);

    GPUResourceFrameTable(GPUResourceFrameTable const&) = delete;
    GPUResourceFrameTable(GPUResourceFrameTable&& rhs);

    GPUResourceFrameTable& operator=(GPUResourceFrameTable const&) = delete;
    GPUResourceFrameTable& operator=(GPUResourceFrameTable&& rhs);

    GPUResourceID InsertResource(std::size_t frameCount, GPUResourceDirectID const* resourceIDs);

    GPUResourceDirectID FetchDirectID(std::size_t frameIndex, GPUResourceID virtualID);

private:
    using FrameContext = std::vector<GPUResourceDirectID>;

    std::vector<FrameContext> frameList_;
    
};