#include <Rendering\Data\Resource\GPUResourceViewContextTable.hpp>

namespace DXRL
{

GPUResourceViewContextTable::GPUResourceViewContextTable() = default;

GPUResourceViewContextTable::GPUResourceViewContextTable(std::size_t framesCount)
{
    for (size_t i = 0; i < framesCount; i++) {
        frameList_.push_back(FrameContext{});
    }
}

GPUResourceViewContextTable::GPUResourceViewContextTable(GPUResourceViewContextTable&& rhs) = default;

GPUResourceViewContextTable& GPUResourceViewContextTable::operator=(GPUResourceViewContextTable&& rhs) = default;

GPUResourceViewHandle GPUResourceViewContextTable::InsertView(std::size_t frameCount, GPUResourceViewDirectHandle const* IDs, GPUResourceViewAllocator& viewAllocator)
{
    auto const contextCount = frameList_.size();

    assert(frameCount > 0 && "Frames count can't be zero");
    assert(((frameCount == contextCount) || (frameCount == 1)) && "Count of resource frames representations does not match context count in resource table.");

    if (frameCount == 1) {
        for (std::size_t i = 0; i < contextCount; i++) {
            frameList_[i].push_back(*IDs);
        }
    }
    else {
        for (size_t i = 0; i < frameCount; i++) {
            frameList_[i].push_back(IDs[i]);
        }
    }

    return GPUResourceViewHandle{ virtualHandleCounter_++, *this, viewAllocator };
}

GPUResourceViewDirectHandle GPUResourceViewContextTable::FetchDirectHandle(std::size_t frameIndex, GPUResourceViewHandle const& virtualHandle)
{
    return frameList_[frameIndex][virtualHandle.ID()];
}

}