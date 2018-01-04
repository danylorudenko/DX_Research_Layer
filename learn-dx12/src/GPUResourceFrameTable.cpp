#include <Rendering\Data\Resource\GPUResourceFrameTable.hpp>
#include <Rendering\Data\Resource\GPUResourceAllocator.hpp>

GPUResourceDirectID::GPUResourceDirectID() = default;

GPUResourceDirectID::GPUResourceDirectID(GPUResourceAllocator& rhs) :
    ID_{ rhs.ProvideNextResourceHandle() }
{ }

GPUResourceDirectID::GPUResourceDirectID(GPUResourceDirectID const&) = default;

GPUResourceDirectID::GPUResourceDirectID(GPUResourceDirectID&&) = default;

GPUResourceDirectID& GPUResourceDirectID::operator=(GPUResourceDirectID const&) = default;

GPUResourceDirectID& GPUResourceDirectID::operator=(GPUResourceDirectID&&) = default;

///////////////////////////////////////////////////////////////////////////////////////////

GPUResourceID::GPUResourceID() = default;

GPUResourceID::GPUResourceID(GPUResourceFrameTable& frameTable) :
    ID_{ frameTable.}

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