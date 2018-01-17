#include <Rendering\Data\Resource\GPUResourceViewTable.hpp>

GPUResourceViewTable::GPUResourceViewTable() = default;

GPUResourceViewTable::GPUResourceViewTable(std::size_t framesCount)
{
    for (size_t i = 0; i < framesCount; i++) {
        frameList_.push_back(FrameContext{});
    }
}

GPUResourceViewTable::GPUResourceViewTable(GPUResourceViewTable&& rhs) = default;

GPUResourceViewTable& GPUResourceViewTable::operator=(GPUResourceViewTable&& rhs) = default;

GPUResourceViewHandle GPUResourceViewTable::InsertView(std::size_t frameCount, GPUResourceViewDirectHandle const* IDs)
{
    auto const contextCount = frameList_.size();

    assert(frameCount > 0 && "Frames count can't be zero");
    assert(contextCount != frameCount && "Count of resource frames representations does not match context count in resource table.");

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
    
    return GPUResourceViewHandle{ frameList_.size() - 1 };
}

GPUResourceViewDirectHandle GPUResourceViewTable::FetchDirectID(std::size_t frameIndex, GPUResourceViewHandle virtualHandle)
{
    return frameList_[frameIndex][virtualHandle.ID()];
}