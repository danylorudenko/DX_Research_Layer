#pragma once

#include <pch.hpp>

class GPUResourceAllocator;

class GPUResourceDirectID
{
public:
    GPUResourceDirectID();
    GPUResourceDirectID(GPUResourceAllocator& allocator);

    GPUResourceDirectID(GPUResourceDirectID const& rhs);
    GPUResourceDirectID(GPUResourceDirectID&& rhs);

    GPUResourceDirectID& operator=(GPUResourceDirectID const& rhs);
    GPUResourceDirectID& operator=(GPUResourceDirectID&& rhs);

private:
    std::size_t ID_ = 0;
};

class GPUResourceID
{
    GPUResourceID();
    GPUResourceID(GPUResourceFrameTable& frameTable);

    GPUResourceID(GPUResourceID const& rhs);
    GPUResourceID(GPUResourceID&& rhs);

    GPUResourceID& operator=(GPUResourceID const& rhs);
    GPUResourceID& operator=(GPUResourceID&& rhs);

private:
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

    GPUResourceID InsertFramedResource(std::size_t frameCount, GPUResourceDirectID* resourceIDs);

    GPUResourceDirectID RecieveResourceExplicitID(std::size_t frameIndex, GPUResourceID virtualID);

private:
    using FrameContext = std::vector<GPUResourceDirectID>;

    std::vector<FrameContext> frameList_;
    
};