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
*               - Alloc(Size size, Size alignment);
*               - AllocArray(Size count, Size unitSize, Size unitAlignment);
*   (opt.)      - Alloc<T>(...);
*   (opt.)      - AllocArray<T>(Size count, ...);
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
class LinearAllocator
{
public:
    DXRL_DEFINE_UNCOPYABLE_MOVABLE(LinearAllocator)

    LinearAllocator(VoidPtr chunk, Size size, bool isOwner = false);
    ~LinearAllocator();

    VoidPtr Alloc(Size size, Size alignment = 4);
    VoidPtr AllocArray(Size count, Size unitSize, Size unitAlignment = 4);

    void Free(VoidPtr data);
    void FreeArray(VoidPtr data);

    void FreeAll();
    void Reset();

    bool IsNull() const;
    Size ChunkSize() const;


    template<typename TResult, typename ...TArgs>
    TResult* Alloc(TArgs&&... args)
    {
        Size constexpr typeSize = sizeof(TResult);
        Size constexpr typeAlignment = alignof(TResult);

        VoidPtr const allocationResult = Alloc(typeSize, typeAlignment);
        return new (allocationResult) TResult(args...);
    }

    template<typename TResult, typename... TArgs>
    TResult* AllocArray(Size count = 1, TArgs&&... args)
    {
        Size constexpr typeSize = sizeof(TResult);
        Size constexpr typeAlignment = alignof(TResult);

        TResult* arrayStart = reinterpret_cast<TResult*>(AllocArray(count, typeSize, typeAlignment));

        for (Size i = 0; i < count; i++)
        {
            new (arrayStart + i) TResult{ args... };
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
    Size mainChunkSize_;
    bool isOwner_;

    VoidPtr freeAddress_;
};



////////////////////////////////////////
class StackAllocator
{
private:

    struct AllocHeader
    {
        Size unitCount_;
        Size allocationScope_;
        U16 scopeStartOffset_;
    };


public:
    DXRL_DEFINE_UNCOPYABLE_MOVABLE(StackAllocator)

    StackAllocator(VoidPtr chunk, Size size, bool requiresDestruction = true, bool isOwner = false);
    ~StackAllocator();

    VoidPtr Alloc(Size size, Size alignment = 4);
    VoidPtr AllocArray(Size count, Size unitSize, Size alignment = 4);

    void Free(VoidPtr ptr);
    void FreeArray(VoidPtr ptr);

    void Reset();

    bool IsNull() const;
    Size ChunkSize() const;

    template<typename TResult, typename... TArgs>
    TResult* Alloc(TArgs&&... args)
    {
        VoidPtr allocationResult = Alloc(sizeof(TResult), alignof(TResult));
        return new (allocationResult) TResult{ args... };
    }

    template<typename TResult, typename... TArgs>
    TResult* AllocArray(Size count, TArgs&&... args)
    {
        Size constexpr typeSize = sizeof(TResult);
        Size constexpr typeAlignment = sizeof(TResult);
        
        TResult* arrayStart = reinterpret_cast<TResult*>(AllocArray(count, typeSize, typeAlignment));
        
        for (Size i = 0; i < count; i++)
        {
            new (arrayStart + i) TResult{ args... };
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
        Size const arraySize = GetArraySize(dataArray);
        for (Size i = 0; i < arraySize; ++i) {
            (dataArray + i)->~T();
        }

        FreeArray(reinterpret_cast<VoidPtr>(dataArray));
    }

private:
    Size GetArraySize(VoidPtr data) const;


private:
    VoidPtr mainChunk_;
    Size mainChunkSize_;

    VoidPtr stackTopPtr_;
    Size currentStackScope_;

    bool isOwner_;
    bool requiresDestruction_;

};




////////////////////////////////////////
template<typename T>
class PoolAllocator
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

    PoolAllocator(VoidPtr mainChunk, Size mainChunkSize, bool requiresDestruction = true, bool isOwner = false)
        : mainChunk_{ mainChunk }
        , mainChunkSize_{ mainChunkSize }
        , freeListStart_{ nullptr }
        , poolSize_{ 0 }
        , allocationsCount_{ 0 }
        , isOwner_{ isOwner }
        , requiresDestruction_{ requiresDestruction }
    {
        Size constexpr poolMemberSize = sizeof(PoolMember);
        Size constexpr poolMemberAlignment = alignof(PoolMember);
        
        poolSize_ = (mainChunkSize_ - poolMemberAlignment) / poolMemberSize;

        freeListStart_ = reinterpret_cast<PoolMember*>(PtrAlign(mainChunk_, poolMemberAlignment));
        Size const iEnd = poolSize_ - 1;
        for (Size i = 0; i < iEnd; ++i) {
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
        return new (result) T{ args... };
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
    Size ChunkSize() const { return mainChunkSize_; }

    Size PoolSize() const { return poolSize_; }
    

private:
    VoidPtr mainChunk_;
    Size mainChunkSize_;

    PoolMember* freeListStart_;
    Size poolSize_;
    Size allocationsCount_;

    bool isOwner_;
    bool requiresDestruction_;
};



////////////////////////////////////////
class FreeListAllocator
{
private:

    enum AllocType : Byte
    {
        Single,
        Array
    };

    struct AllocationHeader
    {
        Size blockSize_;
        Size unitCount_;
        U16 freeBlockStartOffset_;
    };

    struct FreeBlockHeader
    {
        Size size_;
        FreeBlockHeader* nextFreeBlock_;

        bool IsStartAdjacent(FreeBlockHeader* block);
        bool IsEndAdjacent(FreeBlockHeader* block);

        bool TryMergeWithPrevBlock(FreeBlockHeader* block);
        bool TryMergeWithNextBlock(FreeBlockHeader* block);
    };

    static Size constexpr MIN_FREEBLOCK_TAIL = sizeof(FreeBlockHeader) + 16;

public:
    DXRL_DEFINE_UNCOPYABLE_MOVABLE(FreeListAllocator)

    FreeListAllocator(VoidPtr mainChunk, Size mainChunkSize, bool isOwner = false);
    ~FreeListAllocator();

    VoidPtr Alloc(Size size, Size alignment = 4);
    VoidPtr AllocArray(Size count, Size unitSize, Size unitAlignment = 4);

    void Free(VoidPtr data);
    void FreeArray(VoidPtr data);

    void Reset();

    bool IsNull() const;
    Size ChunkSize() const;

    template<typename TResult, typename... TArgs>
    TResult* Alloc(TArgs&&... args)
    {
        VoidPtr result = Alloc(sizeof(TResult), alignof(TResult));
        return new (result) TResult{ args... };
    }

    template<typename TResult, typename... TArgs>
    TResult* AllocArray(Size count, TArgs&&... args)
    {
        TResult* result = reinterpret_cast<TResult*>(AllocArray(count, sizeof(TResult), alignof(TResult)));
        for (Size i = 0; i < count; ++i) {
            new (result + i) TResult{ args... };
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
        Size const arraySize = GetArraySize(data);
        for (Size i = 0; i < arraySize; ++i) {
            (data + i)->~T();
        }
        FreeArray(reinterpret_cast<VoidPtr>(data));
    }

    
private:
    Size GetArraySize(VoidPtr data) const;


private:
    VoidPtr mainChunk_;
    Size mainChunkSize_;

    FreeBlockHeader* firstFreeBlock_;

    bool isOwner_;
};


} // namespace Memory

} // namespace DXRL