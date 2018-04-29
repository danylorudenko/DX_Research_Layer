#pragma once

#include <Foundation\Macro.hpp>
#include <Memory\Bytes.hpp>
#include <Memory\Pointer.hpp>

namespace DXRL
{

namespace Memory
{

////////////////////////////////////////
class LinearAllocator
{
public:
    DXRL_DEFINE_UNCOPYABLE_MOVABLE(LinearAllocator)

    LinearAllocator(VoidPtr chunk, Size size, bool isOwner = false);
    ~LinearAllocator();

    VoidPtr Alloc(Size size, Size alignment = 4);
    VoidPtr AllocArray(Size unitSize, Size count = 1, Size unitAlignment = 4);

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

        TResult* arrayStart = reinterpret_cast<TResult*>(AllocArray(typeSize, count, typeAlignment));

        for (Size i = 0; i < count; i++)
        {
            new (arrayStart + i) TResult{ args... };
        }

        return arrayStart;
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
public:
    DXRL_DEFINE_UNCOPYABLE_MOVABLE(StackAllocator)

    StackAllocator(VoidPtr chunk, Size size, bool requiresDestruction = true, bool isOwner = false);
    ~StackAllocator();

    VoidPtr Alloc(Size size, Size alignment = 0);
    VoidPtr AllocArray(Size unitSize, Size Count = 1, Size alignment = 4);

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
        
        TResult* arrayStart = reinterpret_cast<TResult*>(AllocArray(typeSize, count, typeAlignment));
        
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
    void FreeArray(Size count, T* dataArray)
    {
        for (Size i = 0; i < count; ++i) {
            (dataArray + i)->~T();
        }
    }


private:
    VoidPtr mainChunk_;
    Size mainChunkSize_;

    VoidPtr stackTopPtr_;
    Size currentStackScope_;

    bool isOwner_;
    bool requiresDestruction_;

private:
    
    struct AllocHeaderSingle
    {
        Size allocationSize_;
    };

    struct AllocHeaderArray
    {
        Size unitSize_;
        Size unitsCount_;
    };

    struct AllocHeader
    {
        enum AllocType : Byte
        {
            Single,
            Array
        };
        
        
        Size allocationScope_;
        union
        {
            AllocHeaderSingle singleHeader_;
            AllocHeaderArray arrayHeader_;
        };

        AllocType type_;
    };
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

    struct AllocationHeader
    {
        U16 freeBlockStartOffset_;
    };

    struct FreeBlock
    {
        Size size_;
        FreeBlock* nextFreeBlock_;
    };

    static Size constexpr MIN_FREEBLOCK_TAIL = sizeof(FreeBlock) + 16;

public:
    DXRL_DEFINE_UNCOPYABLE_MOVABLE(FreeListAllocator)

    FreeListAllocator(VoidPtr mainChunk, Size mainChunkSize, bool isOwner);
    ~FreeListAllocator();

    VoidPtr Alloc(Size size, Size alignment);
    void Free(VoidPtr ptr);

    void Reset();

    bool IsNull() const;
    Size ChunkSize() const;

private:
    VoidPtr mainChunk_;
    Size mainChunkSize_;

    FreeBlock* firstFreeBlock_;

    bool isOwner_;
};


} // namespace Memory

} // namespace DXRL