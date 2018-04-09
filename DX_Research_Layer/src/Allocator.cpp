#include <pch.hpp>

#include <Memory\Memory.hpp>

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

Size LinearAllocator::CalculateSize(Size size, Size alignment)
{

}

} // namespace Memory

} // namespace DXRL