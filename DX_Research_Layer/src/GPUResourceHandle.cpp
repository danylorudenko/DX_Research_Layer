#include <Rendering\Resource\GPUResourceHandle.hpp>
#include <Rendering\Resource\GPUStaticResourceAllocator.hpp>
#include <Rendering\Resource\GPUResourceViewContextTable.hpp>
#include <Rendering\Resource\ResourceView\GPUResourceViewAllocator.hpp>

namespace DXRL
{

GPUResourceHandle::GPUResourceHandle() = default;

GPUResourceHandle::GPUResourceHandle(std::size_t ID, GPUStaticResourceAllocator& allocator) :
    ID_{ ID }, allocator_{ &allocator }
{ }

GPUResourceHandle::GPUResourceHandle(GPUResourceHandle const&) = default;

GPUResourceHandle::GPUResourceHandle(GPUResourceHandle&&) = default;

GPUResourceHandle& GPUResourceHandle::operator=(GPUResourceHandle const&) = default;

GPUResourceHandle& GPUResourceHandle::operator=(GPUResourceHandle&&) = default;

std::size_t GPUResourceHandle::ID() const { return ID_; }

GPUResource& GPUResourceHandle::Resource() const
{
    return allocator_->AccessResource(*this);
}

/////////////////////////////////////////////////////////////////////////////////////////

GPUResourceViewHandle::GPUResourceViewHandle() = default;

GPUResourceViewHandle::GPUResourceViewHandle(std::size_t ID, GPUResourceViewContextTable& viewTable, GPUResourceViewAllocator& allocator) :
    ID_{ ID }, viewContextTable_{ &viewTable }, viewAllocator_{ &allocator }
{ }

GPUResourceViewHandle::GPUResourceViewHandle(GPUResourceViewHandle const& rhs) = default;

GPUResourceViewHandle::GPUResourceViewHandle(GPUResourceViewHandle&& rhs) = default;

GPUResourceViewHandle& GPUResourceViewHandle::operator=(GPUResourceViewHandle const& rhs) = default;

GPUResourceViewHandle& GPUResourceViewHandle::operator=(GPUResourceViewHandle&& rhs) = default;

std::size_t GPUResourceViewHandle::ID() const { return ID_; }

bool GPUResourceViewHandle::IsValid() const
{
    return ID_ != (std::numeric_limits<std::size_t>::max)();
}

GPUResourceView& GPUResourceViewHandle::View(std::size_t frameIndex) const
{
    auto directHandle = viewContextTable_->FetchDirectHandle(frameIndex, *this);
    return viewAllocator_->AccessView(directHandle);
}

///////////////////////////////////////////////////////////////////////////////////////////

GPUResourceViewDirectHandle::GPUResourceViewDirectHandle() = default;

GPUResourceViewDirectHandle::GPUResourceViewDirectHandle(std::size_t ID) :
    ID_{ ID }
{ }

GPUResourceViewDirectHandle::GPUResourceViewDirectHandle(GPUResourceViewDirectHandle const& rhs) = default;

GPUResourceViewDirectHandle::GPUResourceViewDirectHandle(GPUResourceViewDirectHandle&& rhs) = default;

GPUResourceViewDirectHandle& GPUResourceViewDirectHandle::operator=(GPUResourceViewDirectHandle const& rhs) = default;

GPUResourceViewDirectHandle& GPUResourceViewDirectHandle::operator=(GPUResourceViewDirectHandle&& rhs) = default;

std::size_t GPUResourceViewDirectHandle::ID() const { return ID_; }

bool GPUResourceViewDirectHandle::IsValid() const
{
    return ID_ != (std::numeric_limits<std::size_t>::max)();
}

}