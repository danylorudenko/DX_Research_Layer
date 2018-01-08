#include <Rendering\Data\Resource\GPUResourceFrameTable.hpp>
#include <Rendering\Data\Resource\GPUResourceAllocator.hpp>


GPUResourceID::GPUResourceID() = default;

GPUResourceID::GPUResourceID(std::size_t ID) :
    ID_{ ID }
{ }

GPUResourceID::GPUResourceID(GPUResourceID const& rhs) = default;

GPUResourceID::GPUResourceID(GPUResourceID&& rhs) = default;

GPUResourceID& GPUResourceID::operator=(GPUResourceID const& rhs) = default;

GPUResourceID& GPUResourceID::operator=(GPUResourceID&& rhs) = default;

///////////////////////////////////////////////////////////////////////////////////////////
GPUResourceFrameTable::GPUResourceFrameTable() = default;

GPUResourceFrameTable::GPUResourceFrameTable(std::size_t framesCount)
{
    for (size_t i = 0; i < framesCount; i++) {
        frameList_.push_back(FrameContext{});
    }
}

GPUResourceFrameTable::GPUResourceFrameTable(GPUResourceFrameTable&& rhs) = default;

GPUResourceFrameTable& GPUResourceFrameTable::operator=(GPUResourceFrameTable&& rhs) = default;

GPUResourceID GPUResourceFrameTable::InsertResource(std::size_t frameCount, GPUResourceDirectID const* IDs)
{
    assert(frameCount > 0 && "Frames count can't be zero");
    
    auto const contextCount = frameList_.size();

    if (frameCount == 1) {
        
        for (std::size_t i = 0; i < contextCount; i++) {
            frameList_[i].push_back(*IDs);
        }
    }
    else {
        assert(contextCount != frameCount && "Count of resource frames representations does not match context count in resource table.");

        for (size_t i = 0; i < frameCount; i++) {
            frameList_[i].push_back(IDs[i]);
        }

    }
    
    return GPUResourceID{ frameList_.size() - 1 };
}

GPUResourceDirectID GPUResourceFrameTable::FetchResourceExplicitID(std::size_t frameIndex, GPUResourceID virtualID)
{
    return frameList_[frameIndex][virtualID.ID_];
}