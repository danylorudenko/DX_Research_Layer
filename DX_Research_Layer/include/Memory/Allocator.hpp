#pragma once

#include <Memory\Bytes.hpp>
#include <Memory\Pointer.hpp>

namespace DXRL
{

namespace Memory
{


/************************************
* ==== Generic Allocator interface ====
*
*               - Alloc(Size_t size, Size_t alignment);
*               - AllocArray(Size_t count, Size_t unitSize, Size_t unitAlignment);
*   (opt.)      - Alloc<T>(...);
*   (opt.)      - AllocArray<T>(Size_t count, ...);
*               
*          
*          
*               - Free(VoidPtr data);
*               - FreeArray(VoidPtr data);
*   (opt.)      - Free<T>(T* data);
*   (opt.)      - FreeArray<T>(T* data);
*
*
*
*
* ==== Exception: PoolAllocator ====
*   
*   - No generic allocator intarface
*   - Pop();
*   - Push(T*);
*
*
************************************/





////////////////////////////////////////
class LinearAllocator final
{
public:
    DXRL_DEFINE_UNCOPYABLE_MOVABLE(LinearAllocator)

    LinearAllocator(VoidPtr chunk, Size_t size, bool isOwner = false);
    ~LinearAllocator();

    VoidPtr Alloc(Size_t size, Size_t alignment = 4);
    VoidPtr AllocArray(Size_t count, Size_t unitSize, Size_t unitAlignment = 4);

    void Free(VoidPtr data);
    void FreeArray(VoidPtr data);

    void FreeAll();
    void Reset();

    bool IsNull() const;
    Size_t ChunkSize() const;


    template<typename TResult, typename ...TArgs>
    TResult* Alloc(TArgs&&... args)
    {
        Size_t constexpr typeSize = sizeof(TResult);
        Size_t constexpr typeAlignment = alignof(TResult);

        VoidPtr const allocationResult = Alloc(typeSize, typeAlignment);
        return new (allocationResult) TResult{ std::forward(args)... };
    }

    template<typename TResult, typename... TArgs>
    TResult* AllocArray(Size_t count = 1, TArgs&&... args)
    {
        Size_t constexpr typeSize = sizeof(TResult);
        Size_t constexpr typeAlignment = alignof(TResult);

        TResult* arrayStart = reinterpret_cast<TResult*>(AllocArray(count, typeSize, typeAlignment));

        for (Size_t i = 0; i < count; i++)
        {
            new (arrayStart + i) TResult{ std::forward(args)... };
        }

        return arrayStart;
    }

    template<typename T>
    void Free(T* data) 
    {
        // INVALID OPERATION
    }

    template<typename T>
    void FreeArray(T* data) 
    {
        // INVALID OPERATION
    } 


private:
    VoidPtr mainChunk_;
    Size_t mainChunkSize_;
    bool isOwner_;

    VoidPtr freeAddress_;
};



////////////////////////////////////////
class StackAllocator final
{
private:

    struct AllocHeader
    {
        Size_t unitCount_;
        Size_t allocationScope_;
        U16_t scopeStartOffset_;
    };


public:
    DXRL_DEFINE_UNCOPYABLE_MOVABLE(StackAllocator)

    StackAllocator(VoidPtr chunk, Size_t size, bool requiresDestruction = true, bool isOwner = false);
    ~StackAllocator();

    VoidPtr Alloc(Size_t size, Size_t alignment = 4);
    VoidPtr AllocArray(Size_t count, Size_t unitSize, Size_t alignment = 4);

    void Free(VoidPtr ptr);
    void FreeArray(VoidPtr ptr);

    void Reset();

    bool IsNull() const;
    Size_t ChunkSize() const;

    template<typename TResult, typename... TArgs>
    TResult* Alloc(TArgs&&... args)
    {
        VoidPtr allocationResult = Alloc(sizeof(TResult), alignof(TResult));
        return new (allocationResult) TResult{ std::forward(args)... };
    }

    template<typename TResult, typename... TArgs>
    TResult* AllocArray(Size_t count, TArgs&&... args)
    {
        Size_t constexpr typeSize = sizeof(TResult);
        Size_t constexpr typeAlignment = sizeof(TResult);
        
        TResult* arrayStart = reinterpret_cast<TResult*>(AllocArray(count, typeSize, typeAlignment));
        
        for (Size_t i = 0; i < count; i++)
        {
            new (arrayStart + i) TResult{ std::forward(args)... };
        }

        return arrayStart;
    }

    template<typename T>
    void Free(T* data)
    {
        data->~T();
        this->Free(reinterpret_cast<VoidPtr>(data));
    }

    template<typename T>
    void FreeArray(T* dataArray)
    {
        Size_t const arraySize = GetArraySize(dataArray);
        for (Size_t i = 0; i < arraySize; ++i) {
            (dataArray + i)->~T();
        }

        FreeArray(reinterpret_cast<VoidPtr>(dataArray));
    }

private:
    Size_t GetArraySize(VoidPtr data) const;


private:
    VoidPtr mainChunk_;
    Size_t mainChunkSize_;

    VoidPtr stackTopPtr_;
    Size_t currentStackScope_;

    bool isOwner_;
    bool requiresDestruction_;

};




////////////////////////////////////////
template<typename T>
class PoolAllocator final
{
private:
    struct PoolMember
    {
        T data_;
        PoolMember* nextFree_;
    };

public:
    PoolAllocator(PoolAllocator<T> const&) = delete;
    PoolAllocator<T>& operator=(PoolAllocator<T> const& rhs) = delete;

    PoolAllocator()
        : mainChunk_{ nullptr }
        , mainChunkSize_{ 0 }
        , freeListStart_{ nullptr }
        , poolSize_{ 0 }
        , allocationsCount_{ 0 }
        , isOwner_{ false }
        , requiresDestruction_{ true }
    { }

    PoolAllocator(VoidPtr mainChunk, Size_t mainChunkSize, bool requiresDestruction = true, bool isOwner = false)
        : mainChunk_{ mainChunk }
        , mainChunkSize_{ mainChunkSize }
        , freeListStart_{ nullptr }
        , poolSize_{ 0 }
        , allocationsCount_{ 0 }
        , isOwner_{ isOwner }
        , requiresDestruction_{ requiresDestruction }
    {
        Size_t constexpr poolMemberSize = sizeof(PoolMember);
        Size_t constexpr poolMemberAlignment = alignof(PoolMember);
        
        poolSize_ = (mainChunkSize_ - poolMemberAlignment) / poolMemberSize;

        freeListStart_ = reinterpret_cast<PoolMember*>(PtrAlign(mainChunk_, poolMemberAlignment));
        Size_t const iEnd = poolSize_ - 1;
        for (Size_t i = 0; i < iEnd; ++i) {
            freeListStart_[i].nextFree_ = (freeListStart_ + (i + 1));
        }
    }

    PoolAllocator(PoolAllocator<T>&& rhs)
        : mainChunk_{ nullptr }
        , mainChunkSize_{ 0 }
        , freeListStart_{ nullptr }
        , poolSize_{ 0 }
        , allocationsCount_{ 0 }
        , isOwner_{ false }
        , requiresDestruction_{ true }
    {
        operator=(std::move(rhs));
    }

    PoolAllocator<T>& operator=(PoolAllocator<T>&& rhs)
    {
        if(!IsNull())
            Reset();
        
        mainChunk_ = rhs.mainChunk_;
        mainChunkSize_ = rhs.mainChunkSize_;
        freeListStart_ = rhs.freeListStart_;
        poolSize_ = rhs.poolSize_;
        allocationsCount = rhs.allocationsCount;
        isOwner_ = rhs.isOwner_;
        requiresDestruction_ = rhs.requiresDestruction_;

        rhs.allocationsCount_ = 0;
        rhs.isOwner_ = false;
        rhs.Reset();
    }

    ~PoolAllocator()
    {
        Reset();
    }

    void Reset()
    {
        assert((requiresDestruction_ ? (allocationsCount_ == 0) : true) && "Can't reset PoolAllocator unless all allocations are released.");
        
        if (isOwner_) 
            free(mainChunk_);

        mainChunk_ = nullptr;
        mainChunkSize_ = 0;

        freeListStart_ = nullptr;
        poolSize_ = 0;
        isOwner_ = false;
        requiresDestruction_ = true;
    }

    template<typename... TArgs>
    T* Pop(TArgs&&... args)
    {
        assert( allocationsCount_ < poolSize_ && "No more free members in the pool.");

        T* result = reinterpret_cast<T*>(freeListStart_);
        freeListStart_ = freeListStart_->nextFree_;

        ++allocationsCount_;
        return new (result) T{ std::forward(args)... };
    }

    void Push(T* data)
    {
        data->~T();

        PoolMember* newFreeMember = reinterpret_cast<PoolMember*>(data);
        newFreeMember->nextFree_ = freeListStart_;
        freeListStart_ = newFreeMember;
        --allocationsCount_;
    }

    bool IsNull() const { return mainChunk_ == nullptr; }
    Size_t ChunkSize() const { return mainChunkSize_; }

    Size_t PoolSize() const { return poolSize_; }
    

private:
    VoidPtr mainChunk_;
    Size_t mainChunkSize_;

    PoolMember* freeListStart_;
    Size_t poolSize_;
    Size_t allocationsCount_;

    bool isOwner_;
    bool requiresDestruction_;
};



////////////////////////////////////////
class FreeListAllocator final
{
private:

    enum AllocType : Byte
    {
        Single,
        Array
    };

    struct AllocationHeader
    {
        Size_t blockSize_;
        Size_t unitCount_;
        U16_t freeBlockStartOffset_;
    };

    struct FreeBlockHeader
    {
        Size_t size_;
        FreeBlockHeader* nextFreeBlock_;

        bool IsStartAdjacent(FreeBlockHeader* block);
        bool IsEndAdjacent(FreeBlockHeader* block);

        bool TryMergeWithPrevBlock(FreeBlockHeader* block);
        bool TryMergeWithNextBlock(FreeBlockHeader* block);
    };

    static Size_t constexpr MIN_FREEBLOCK_TAIL = sizeof(FreeBlockHeader) + 16;

public:
    DXRL_DEFINE_UNCOPYABLE_MOVABLE(FreeListAllocator)

    FreeListAllocator(VoidPtr mainChunk, Size_t mainChunkSize, bool isOwner = false);
    ~FreeListAllocator();

    VoidPtr Alloc(Size_t size, Size_t alignment = 4);
    VoidPtr AllocArray(Size_t count, Size_t unitSize, Size_t unitAlignment = 4);

    void Free(VoidPtr data);
    void FreeArray(VoidPtr data);

    void Reset();

    bool IsNull() const;
    Size_t ChunkSize() const;

    template<typename TResult, typename... TArgs>
    TResult* Alloc(TArgs&&... args)
    {
        VoidPtr result = Alloc(sizeof(TResult), alignof(TResult));
        return new (result) TResult{ std::forward(args)... };
    }

    template<typename TResult, typename... TArgs>
    TResult* AllocArray(Size_t count, TArgs&&... args)
    {
        TResult* result = reinterpret_cast<TResult*>(AllocArray(count, sizeof(TResult), alignof(TResult)));
        for (Size_t i = 0; i < count; ++i) {
            new (result + i) TResult{ std::forward(args)... };
        }
    }

    template<typename T>
    void Free(T* data)
    {
        data->~T();
        Free(reinterpret_cast<VoidPtr>(data));
    }

    template<typename T>
    void FreeArray(T* data)
    {
        Size_t const arraySize = GetArraySize(data);
        for (Size_t i = 0; i < arraySize; ++i) {
            (data + i)->~T();
        }
        FreeArray(reinterpret_cast<VoidPtr>(data));
    }

    
private:
    Size_t GetArraySize(VoidPtr data) const;


private:
    VoidPtr mainChunk_;
    Size_t mainChunkSize_;

    FreeBlockHeader* firstFreeBlock_;

    bool isOwner_;
};


} // namespace Memory

} // namespace DXRL