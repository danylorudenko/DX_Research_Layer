#pragma once

#include <pch.hpp>

class GPUResourceDirectID;

class GPUResourceID
{
public:
    GPUResourceID();
    GPUResourceID(std::size_t ID);

    GPUResourceID(GPUResourceID const& rhs);
    GPUResourceID(GPUResourceID&& rhs);

    GPUResourceID& operator=(GPUResourceID const& rhs);
    GPUResourceID& operator=(GPUResourceID&& rhs);

    std::size_t ID_ = 0;
};

class GPUResourceFrameTable
{
public:
    GPUResourceFrameTable();
    GPUResourceFrameTable(std::size_t framesCount);

    GPUResourceFrameTable(GPUResourceFrameTable const&) = delete;
    GPUResourceFrameTable(GPUResourceFrameTable&& rhs);

    GPUResourceFrameTable& operator=(GPUResourceFrameTable const&) = delete;
    GPUResourceFrameTable& operator=(GPUResourceFrameTable&& rhs);

    GPUResourceID InsertFramedResource(std::size_t frameCount, GPUResourceDirectID const* resourceIDs);
    GPUResourceID InsertSingleResource(GPUResourceDirectID resourceID);

    GPUResourceDirectID FetchResourceExplicitID(std::size_t frameIndex, GPUResourceID virtualID);

private:
    using FrameContext = std::vector<GPUResourceDirectID>;

    std::vector<FrameContext> frameList_;
    
};