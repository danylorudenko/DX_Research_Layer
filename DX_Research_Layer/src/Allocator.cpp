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

VoidPtr LinearAllocator::AllocArray(Size count, Size unitSize, Size unitAlignment)
{
    return Alloc(unitSize * count, unitAlignment);
}

void LinearAllocator::Free(VoidPtr data)
{
    // INVALID OPERATION
}

void LinearAllocator::FreeArray(VoidPtr data)
{
    // INVALID OPERATION
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
   return AllocArray(1, size, alignment);
}

VoidPtr StackAllocator::AllocArray(Size unitCount, Size unitSize, Size alignment)
{
    Size const allocationSize = CalcSizeWithAlignment(unitSize * unitCount, alignment, sizeof(AllocHeader));

    assert((PtrDifference(PtrAdd(stackTopPtr_, allocationSize), mainChunk_) <= static_cast<PtrDiff>(mainChunkSize_)) && "Allocation exceeds chunk size!");

    VoidPtr const headerAdjustedTop = PtrAdd(stackTopPtr_, sizeof(AllocHeader));
    VoidPtr const allocationResult = PtrAlign(headerAdjustedTop, alignment);

    AllocHeader* const headerPtr = reinterpret_cast<AllocHeader*>(PtrNegate(allocationResult, sizeof(AllocHeader)));
    headerPtr->unitSize_ = unitSize;
    headerPtr->unitCount_ = unitCount;
    headerPtr->allocationScope_ = ++currentStackScope_;
    headerPtr->scopeStartOffset_ = static_cast<U16>(PtrDifference(stackTopPtr_, allocationResult));


    stackTopPtr_ = PtrAdd(stackTopPtr_, allocationSize);

    return allocationResult;
}

void StackAllocator::Free(VoidPtr ptr)
{
    FreeArray(ptr);
}

void StackAllocator::FreeArray(VoidPtr arrayPtr)
{
    Size constexpr headerSize = sizeof(AllocHeader);

    AllocHeader const* const header = reinterpret_cast<AllocHeader*>(PtrNegate(arrayPtr, headerSize));
    assert(header->allocationScope_ == currentStackScope_ && "Trying to free not-the-top scope of the stack!");

    stackTopPtr_ = PtrNegate(arrayPtr, header->scopeStartOffset_);
    --currentStackScope_;
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

Size StackAllocator::GetArraySize(VoidPtr data) const
{
    Size constexpr headerSize = sizeof(AllocHeader);
    AllocHeader const* const header = reinterpret_cast<AllocHeader*>(PtrNegate(data, headerSize));
    return header->unitCount_;
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
    firstFreeBlock_ = reinterpret_cast<FreeBlockHeader*>(mainChunk_);
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

    return *this;
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

VoidPtr FreeListAllocator::Alloc(Size size, Size alignment)
{
    if (firstFreeBlock_ == nullptr)
        return nullptr;
    
    Size allocationSize = CalcSizeWithAlignment(size, alignment, sizeof(AllocationHeader));

    FreeBlockHeader* prevBlock = nullptr;
    FreeBlockHeader* validBlock = firstFreeBlock_;
    
    while (validBlock->size_ < allocationSize) {
        prevBlock = validBlock;
        validBlock = validBlock->nextFreeBlock_;

        if (validBlock == nullptr)
            return nullptr;
    }

    Size const validBlockTailSize = validBlock->size_ - allocationSize;
    FreeBlockHeader* const tailFreeBlockHeader = reinterpret_cast<FreeBlockHeader*>(PtrAdd(validBlock, allocationSize));

    if (prevBlock == nullptr) {
        if (validBlockTailSize >= MIN_FREEBLOCK_TAIL) {
            tailFreeBlockHeader->size_ = validBlockTailSize;
            tailFreeBlockHeader->nextFreeBlock_ = validBlock->nextFreeBlock_;

            firstFreeBlock_ = tailFreeBlockHeader;
        }
        else {
            allocationSize = validBlock->size_;
            firstFreeBlock_ = validBlock->nextFreeBlock_;
        }
    }
    else {
        if (validBlockTailSize >= MIN_FREEBLOCK_TAIL) {
            tailFreeBlockHeader->nextFreeBlock_ = validBlock->nextFreeBlock_;
            tailFreeBlockHeader->size_ = validBlockTailSize;

            prevBlock->nextFreeBlock_ = tailFreeBlockHeader;
        }
        else {
            allocationSize = validBlock->size_;
            prevBlock->nextFreeBlock_ = validBlock->nextFreeBlock_;
        }
    }

    VoidPtr const validBlockPlusHeader = PtrAdd(validBlock, sizeof(AllocationHeader));
    VoidPtr const result = PtrAlign(validBlockPlusHeader, alignment);
    AllocationHeader* const header = reinterpret_cast<AllocationHeader*>(PtrNegate(result, sizeof(AllocationHeader)));
    header->allocationSize_ = allocationSize;
    header->freeBlockStartOffset_ = static_cast<U16>(PtrDifference(result, validBlock));

    return result;
}

VoidPtr FreeListAllocator::AllocArray(Size count, Size unitSize, Size unitAlignment)
{
    Size const allocationSize = count * unitSize;
    return Alloc(allocationSize, unitAlignment);
}

void FreeListAllocator::Free(VoidPtr data)
{
    AllocationHeader const* const allocationHeader = reinterpret_cast<AllocationHeader*>(PtrNegate(data, sizeof(AllocationHeader)));
    assert(allocationHeader->type_ == AllocType::Single && "Attempting to free array allocation with regular Free().");

    VoidPtr const blockStart = PtrNegate(data, allocationHeader->freeBlockStartOffset_);
    Size const releasedBlockSize = allocationHeader->allocationSize_;
    FreeBlockHeader* releasedBlockHeader = reinterpret_cast<FreeBlockHeader*>(blockStart);

    
    if (firstFreeBlock_ != nullptr) {
        if (PtrDifference(blockStart, firstFreeBlock_) < 0) {
            // Released block is the earliest

            releasedBlockHeader->size_ = releasedBlockSize;
            if (releasedBlockHeader->TryMergeWithNextBlock(firstFreeBlock_)) {
                releasedBlockHeader->nextFreeBlock_ = firstFreeBlock_->nextFreeBlock_;
                firstFreeBlock_ = releasedBlockHeader;
                return;
            }
            else {
                releasedBlockHeader->nextFreeBlock_ = firstFreeBlock_;
                firstFreeBlock_ = releasedBlockHeader;
                return;
            }
        }

        // Released block is not the earliest

        FreeBlockHeader* prevFreeBlock = firstFreeBlock_;
        while (PtrDifference(prevFreeBlock->nextFreeBlock_, releasedBlockHeader) < 0) {
            prevFreeBlock = prevFreeBlock->nextFreeBlock_;
            if (prevFreeBlock->nextFreeBlock_ == nullptr) {
                // Released block is the last one
                releasedBlockHeader->size_ = releasedBlockSize;
                if (!releasedBlockHeader->TryMergeWithPrevBlock(prevFreeBlock)) {
                    prevFreeBlock->nextFreeBlock_ = releasedBlockHeader;
                }
                return;
            }
        }

        // Released block is intermediate

        releasedBlockHeader->size_ = releasedBlockSize;

        bool const nextMerged = releasedBlockHeader->TryMergeWithNextBlock(prevFreeBlock->nextFreeBlock_);
        bool const prevMerged = releasedBlockHeader->TryMergeWithPrevBlock(prevFreeBlock);

        if (prevMerged && nextMerged) {
            prevFreeBlock->nextFreeBlock_ = prevFreeBlock->nextFreeBlock_->nextFreeBlock_;
        }
        else if (!prevMerged && nextMerged) {
            prevFreeBlock->nextFreeBlock_ = releasedBlockHeader;
        }
        else if (!prevMerged && !nextMerged) {
            releasedBlockHeader->nextFreeBlock_ = prevFreeBlock->nextFreeBlock_;
            prevFreeBlock->nextFreeBlock_ = releasedBlockHeader;
        }
    }
    else {
        firstFreeBlock_ = releasedBlockHeader;
    }
}

void FreeListAllocator::FreeArray(VoidPtr data)
{

}

bool FreeListAllocator::FreeBlockHeader::IsEndAdjacent(FreeListAllocator::FreeBlockHeader* block)
{
    VoidPtr thisEnd = PtrAdd(this, size_);
    return thisEnd == block;
}

bool FreeListAllocator::FreeBlockHeader::IsStartAdjacent(FreeListAllocator::FreeBlockHeader* block)
{
    return block->IsEndAdjacent(this);
}

bool FreeListAllocator::FreeBlockHeader::TryMergeWithPrevBlock(FreeListAllocator::FreeBlockHeader* block)
{
    if (block->IsEndAdjacent(this)) {
        block->size_ += size_;
        return true;
    }
    return false;
}

bool FreeListAllocator::FreeBlockHeader::TryMergeWithNextBlock(FreeListAllocator::FreeBlockHeader* block)
{
    if (IsEndAdjacent(block)) {
        size_ += block->size_;
        return true;
    }
    return false;
}


} // namespace Memory

} // namespace DXRL