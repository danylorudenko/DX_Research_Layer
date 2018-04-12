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
    Size allocationSize = CalcSizeWithAlignment(size, alignment, 0);

    VoidPtr alignedResult = PtrAlign(freeAddress, alignment);
    allocated_ += allocationSize;

    return alignedResult;
}

void LinearAllocator::Reset()
{
    allocated_ = 0;
}

} // namespace Memory

} // namespace DXRL