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
    , freeAddress_{ nullptr }
{ }

LinearAllocator::LinearAllocator(LinearAllocator&& rhs)
{
    operator=(std::move(rhs));
}

LinearAllocator& LinearAllocator::operator=(LinearAllocator&& rhs)
{
    if (!IsNull())
        Reset();

    mainChunk_ = rhs.mainChunk_;
    mainChunkSize_ = rhs.mainChunkSize_;
    isOwner_ = rhs.isOwner_;
    freeAddress_ = rhs.freeAddress_;

    rhs.isOwner_ = false;
    rhs.Reset();
}

LinearAllocator::LinearAllocator(VoidPtr chunk, Size size, bool isOwner)
    : mainChunk_{ nullptr }
    , mainChunkSize_{ size }
    , isOwner_{ isOwner }
    , freeAddress_{ chunk }
{ }

LinearAllocator::~LinearAllocator()
{
    Reset();
}

VoidPtr LinearAllocator::Alloc(Size size, Size alignment)
{
    Size allocationSize = CalcSizeWithAlignment(size, alignment);

    VoidPtr allocationResult = PtrAlign(freeAddress_, alignment);
    freeAddress_ = PtrAdd(freeAddress_, allocationSize);

    assert((PtrDifference(mainChunk_, PtrAdd(freeAddress_, allocationSize)) < mainChunkSize_) && "Allocation exceeds chunk size!");

    return allocationResult;
}

VoidPtr LinearAllocator::AllocArray(Size unitSize, Size count, Size unitAlignment)
{
    return Alloc(unitSize * count, unitAlignment);
}

void LinearAllocator::FreeAll()
{
    freeAddress_ = mainChunk_;
}

void LinearAllocator::Reset()
{
    if(isOwner_)
        delete[] mainChunk_;

    mainChunk_ = nullptr;
    mainChunkSize_ = 0;
    isOwner_ = false;
    freeAddress_ = nullptr;
}

bool LinearAllocator::IsNull() const
{
    return mainChunk_ == nullptr;
}

////////////////////////////////////////
StackAllocator::StackAllocator()
    : mainChunk_{ nullptr }
    , mainChunkSize_{ 0 }
    , isOwner_{ false }
    , stackTopPtr_{ nullptr }
    , currentStackScope_{ 0 }
{ }

StackAllocator::StackAllocator(VoidPtr chunk, Size size, bool isOwner = false)
    : mainChunk_{ chunk }
    , mainChunkSize_{ size }
    , isOwner_{ isOwner }
    , stackTopPtr_{ chunk }
    , currentStackScope_{ 0 }
{ }

StackAllocator::StackAllocator(StackAllocator&& rhs)
{
    operator=(std::move(rhs));
}

StackAllocator& StackAllocator::operator=(StackAllocator&& rhs)
{
    if(!IsNull())
        Reset();
    
    mainChunk_ = rhs.mainChunk_;
    mainChunkSize_ = rhs.mainChunkSize_;
    isOwner_ = rhs.isOwner_;
    stackTopPtr_ = rhs.stackTopPtr_;
    currentStackScope_ = rhs.currentStackScope_;

    rhs.isOwner_ = false;
    rhs.Reset();
}

StackAllocator::~StackAllocator()
{
    Reset();
}

VoidPtr StackAllocator::Alloc(Size size, Size alignment)
{
    Size allocationSize = CalcSizeWithAlignment(size, alignment, sizeof(AllocHeader));
    
    assert((PtrDifference(mainChunk_, PtrAdd(stackTopPtr_, allocationSize)) < mainChunkSize_) && "Allocation exceeds chunk size!");

    VoidPtr const headerAdjustedTop = PtrAdd(stackTopPtr_, sizeof(AllocHeader));
    VoidPtr const allocationResult = PtrAlign(headerAdjustedTop, alignment);

    AllocHeader* const headerPtr = reinterpret_cast<AllocHeader*>(PtrNegate(allocationResult, sizeof(AllocHeader)));
    headerPtr->allocationScope_ = ++currentStackScope_;
    headerPtr->allocationStart_ = stackTopPtr_;
    headerPtr->singleHeader_.allocationSize_ = allocationSize;
    headerPtr->type_ = AllocHeader::Single;

    stackTopPtr_ = PtrAdd(stackTopPtr_, allocationSize);

    return allocationResult;
}

VoidPtr StackAllocator::AllocArray(Size unitSize, Size unitCount, Size alignment)
{
    Size const allocationSize = CalcSizeWithAlignment(unitSize * unitCount, alignment, sizeof(AllocHeader));

    VoidPtr const headerAdjustedTop = PtrAdd(stackTopPtr_, sizeof(AllocHeader));
    VoidPtr const allocationResult = PtrAlign(headerAdjustedTop, alignment);

    AllocHeader* const headerPtr = reinterpret_cast<AllocHeader*>(PtrNegate(allocationResult, sizeof(AllocHeader)));
    headerPtr->allocationScope_ = ++currentStackScope_;
    headerPtr->allocationStart_ = stackTopPtr_;
    headerPtr->arrayHeader_.unitsCount_ = unitCount;
    headerPtr->arrayHeader_.unitSize_ = unitSize;
    headerPtr->type_ = AllocHeader::Array;

    stackTopPtr_ = PtrAdd(stackTopPtr_, allocationSize);

    return allocationResult;
}

void StackAllocator::Free(VoidPtr ptr)
{
    Size constexpr headerSize = sizeof(AllocHeader);

    AllocHeader const* header = reinterpret_cast<AllocHeader*>(PtrNegate(ptr, headerSize));
    assert(header->allocationScope_ == currentStackScope_ && "Trying to free not-the-top scope of the stack!");
    assert(header->type_ == AllocHeader::Single && "Trying to free array-type allocation with regular Free()");

    stackTopPtr_ = header->allocationStart_;
}

void StackAllocator::FreeArray(VoidPtr arrayPtr)
{
    Size constexpr headerSize = sizeof(AllocHeader);

    AllocHeader const* header = reinterpret_cast<AllocHeader*>(PtrNegate(arrayPtr, headerSize));
    assert(header->allocationScope_ == currentStackScope_ && "Trying to free not-the-top scope of the stack!");
    assert(header->type_ == AllocHeader::Array && "Trying to free single-type allocation with FreeArray()");

    stackTopPtr_ = header->allocationStart_;
}

void StackAllocator::Reset()
{
    if(isOwner_)
        delete[] mainChunk_;
    
    mainChunk_ = nullptr;
    mainChunkSize_ = 0;
    isOwner_ = false;
    stackTopPtr_ = nullptr;
    currentStackScope_ = 0;
}

bool StackAllocator::IsNull() const
{
    return mainChunk_ == nullptr;
}

} // namespace Memory

} // namespace DXRL