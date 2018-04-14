#include <pch.hpp>

#include <Memory\Memory.hpp>
#include <Memory\Pointer.hpp>

namespace DXRL
{

namespace Memory
{

LinearAllocator::LinearAllocator()
    : mainChunk_{ nullptr }
    , mainChunkSize_{ 0 }
    , isOwner_{ false }
    , allocated_{ 0 }
{ }

LinearAllocator::LinearAllocator(LinearAllocator&& rhs)
    : mainChunk_{ rhs.mainChunk_ }
    , mainChunkSize_{ rhs.mainChunkSize_ }
    , isOwner_{ rhs.isOwner_ }
    , allocated_{ rhs.allocated_ }
{
    rhs.mainChunk_ = nullptr;
    rhs.mainChunkSize_ = 0;
    rhs.isOwner_ = false;
    rhs.allocated_ = 0;
}

LinearAllocator& LinearAllocator::operator=(LinearAllocator&& rhs)
{
    mainChunk_ = rhs.mainChunk_;            rhs.mainChunk_ = nullptr;
    mainChunkSize_ = rhs.mainChunkSize_;    rhs.mainChunkSize_ = 0;
    isOwner_ = rhs.isOwner_;                rhs.isOwner_ = false;
    allocated_ = rhs.allocated_;            rhs.allocated_ = 0;
}

LinearAllocator::LinearAllocator(VoidPtr chunk, Size size, bool isOwner)
    : mainChunk_{ nullptr }
    , mainChunkSize_{ size }
    , isOwner_{ isOwner }
    , allocated_{ 0 }
{ }

LinearAllocator::~LinearAllocator()
{
    if (isOwner_)
        delete[] mainChunk_;
}

VoidPtr LinearAllocator::Alloc(Size size, Size alignment)
{
    VoidPtr freeAddress = PtrAdd(mainChunk_, allocated_);
    Size allocationSize = CalcSizeWithAlignment(size, alignment);

    VoidPtr alignedResult = PtrAlign(freeAddress, alignment);
    allocated_ += allocationSize;

    return alignedResult;
}

VoidPtr LinearAllocator::AllocArray(Size unitSize, Size count, Size unitAlignment)
{
    return Alloc(unitSize * count, unitAlignment);
}

void LinearAllocator::Reset()
{
    allocated_ = 0;
}

} // namespace Memory

} // namespace DXRL