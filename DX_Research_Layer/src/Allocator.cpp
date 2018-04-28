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
    , freeAddress_{ nullptr }
{ }

LinearAllocator::LinearAllocator(LinearAllocator&& rhs)
    : mainChunk_{ nullptr }
    , mainChunkSize_{ 0 }
    , isOwner_{ false }
    , freeAddress_{ nullptr }
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

    return *this;
}

LinearAllocator::LinearAllocator(VoidPtr chunk, Size size, bool isOwner)
    : mainChunk_{ chunk }
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

    assert((PtrDifference(PtrAdd(freeAddress_, allocationSize), mainChunk_) <= static_cast<PtrDiff>(mainChunkSize_)) && "Allocation exceeds chunk size!");

    VoidPtr allocationResult = PtrAlign(freeAddress_, alignment);
    freeAddress_ = PtrAdd(freeAddress_, allocationSize);


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
        free(mainChunk_);

    mainChunk_ = nullptr;
    mainChunkSize_ = 0;
    isOwner_ = false;
    freeAddress_ = nullptr;
}

bool LinearAllocator::IsNull() const
{
    return mainChunk_ == nullptr;
}

Size LinearAllocator::ChunkSize() const
{
    return mainChunkSize_;
}

////////////////////////////////////////
StackAllocator::StackAllocator()
    : mainChunk_{ nullptr }
    , mainChunkSize_{ 0 }
    , stackTopPtr_{ nullptr }
    , currentStackScope_{ 0 }
    , isOwner_{ false }
    , requiresDestruction_{ true }
{ }

StackAllocator::StackAllocator(VoidPtr chunk, Size size, bool requiresDestruction, bool isOwner)
    : mainChunk_{ chunk }
    , mainChunkSize_{ size }
    , stackTopPtr_{ chunk }
    , currentStackScope_{ 0 }
    , isOwner_{ isOwner }
    , requiresDestruction_{ requiresDestruction }
{ }

StackAllocator::StackAllocator(StackAllocator&& rhs)
    : mainChunk_{ nullptr }
    , mainChunkSize_{ 0 }
    , stackTopPtr_{ nullptr }
    , currentStackScope_{ 0 }
    , isOwner_{ false }
    , requiresDestruction_{ true }
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

    return *this;
}

StackAllocator::~StackAllocator()
{
    Reset();
}

VoidPtr StackAllocator::Alloc(Size size, Size alignment)
{
    Size allocationSize = CalcSizeWithAlignment(size, alignment, sizeof(AllocHeader));
    
    assert((PtrDifference(PtrAdd(stackTopPtr_, allocationSize), mainChunk_) <= static_cast<PtrDiff>(mainChunkSize_)) && "Allocation exceeds chunk size!");

    VoidPtr const headerAdjustedTop = PtrAdd(stackTopPtr_, sizeof(AllocHeader));
    VoidPtr const allocationResult = PtrAlign(headerAdjustedTop, alignment);

    AllocHeader* const headerPtr = reinterpret_cast<AllocHeader*>(PtrNegate(allocationResult, sizeof(AllocHeader)));
    headerPtr->allocationScope_ = ++currentStackScope_;
    headerPtr->singleHeader_.allocationSize_ = allocationSize;
    headerPtr->type_ = AllocHeader::Single;

    stackTopPtr_ = PtrAdd(stackTopPtr_, allocationSize);

    return allocationResult;
}

VoidPtr StackAllocator::AllocArray(Size unitSize, Size unitCount, Size alignment)
{
    Size const allocationSize = CalcSizeWithAlignment(unitSize * unitCount, alignment, sizeof(AllocHeader));

    assert((PtrDifference(PtrAdd(stackTopPtr_, allocationSize), mainChunk_) <= static_cast<PtrDiff>(mainChunkSize_)) && "Allocation exceeds chunk size!");

    VoidPtr const headerAdjustedTop = PtrAdd(stackTopPtr_, sizeof(AllocHeader));
    VoidPtr const allocationResult = PtrAlign(headerAdjustedTop, alignment);

    AllocHeader* const headerPtr = reinterpret_cast<AllocHeader*>(PtrNegate(allocationResult, sizeof(AllocHeader)));
    headerPtr->allocationScope_ = ++currentStackScope_;
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

    --currentStackScope_;
    stackTopPtr_ = PtrNegate(stackTopPtr_, header->singleHeader_.allocationSize_);
}

void StackAllocator::FreeArray(VoidPtr arrayPtr)
{
    Size constexpr headerSize = sizeof(AllocHeader);

    AllocHeader const* header = reinterpret_cast<AllocHeader*>(PtrNegate(arrayPtr, headerSize));
    assert(header->allocationScope_ == currentStackScope_ && "Trying to free not-the-top scope of the stack!");
    assert(header->type_ == AllocHeader::Array && "Trying to free single-type allocation with FreeArray()");

    --currentStackScope_;
    Size const allocationSize = header->arrayHeader_.unitSize_ * header->arrayHeader_.unitSize_;
    stackTopPtr_ = PtrNegate(stackTopPtr_, allocationSize);
}

void StackAllocator::Reset()
{
    assert((requiresDestruction_ ? (currentStackScope_ == 0) : true) && "Stack allocator must be unwinded before destruction.");

    if(isOwner_)
        free(mainChunk_);
    
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

Size StackAllocator::ChunkSize() const
{
    return mainChunkSize_;
}



////////////////////////////////////////
FreeListAllocator::FreeListAllocator()
    : mainChunk_{ nullptr }
    , mainChunkSize_{ 0 }
    , firstFreeBlock_{ nullptr }
    , isOwner_{ false }
{ }

FreeListAllocator::FreeListAllocator(VoidPtr mainChunk, Size mainChunkSize, bool isOwner)
    : mainChunk_{ mainChunk }
    , mainChunkSize_{ mainChunkSize }
    , firstFreeBlock_{ nullptr }
    , isOwner_{ isOwner }
{
    firstFreeBlock_ = reinterpret_cast<FreeBlock*>(mainChunk_);
    firstFreeBlock_->size_ = mainChunkSize_;
    firstFreeBlock_->nextFreeBlock_ = nullptr;
}

FreeListAllocator::FreeListAllocator(FreeListAllocator&& rhs)
    : mainChunk_{ nullptr }
    , mainChunkSize_{ 0 }
    , firstFreeBlock_{ nullptr }
    , isOwner_{ false }
{
    operator=(std::move(rhs));
}

FreeListAllocator& FreeListAllocator::operator=(FreeListAllocator&& rhs)
{
    if(!IsNull())
        Reset();

    mainChunk_ = rhs.mainChunk_;
    mainChunkSize_ = rhs.mainChunkSize_;
    firstFreeBlock_ = rhs.firstFreeBlock_;
    isOwner_ = rhs.isOwner_;

    rhs.isOwner_ = false;
    rhs.Reset();
}

FreeListAllocator::~FreeListAllocator()
{
    Reset();
}

bool FreeListAllocator::IsNull() const
{
    return mainChunk_ == nullptr;
}

Size FreeListAllocator::ChunkSize() const
{
    return mainChunkSize_;
}

void FreeListAllocator::Reset()
{
    if(isOwner_)
        free(mainChunk_);

    mainChunk_ = nullptr;
    mainChunkSize_ = 0;
    firstFreeBlock_ = nullptr;
    isOwner_ = false;
}

} // namespace Memory

} // namespace DXRL