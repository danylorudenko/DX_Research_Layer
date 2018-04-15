#pragma once

#include <Foundation\Macro.hpp>
#include <Memory\Bytes.hpp>

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

    StackAllocator(VoidPtr chunk, Size size, bool isOwner = false);
    ~StackAllocator();

    VoidPtr Alloc(Size size, Size alignment = 0);
    VoidPtr AllocArray(Size unitSize, Size unitCount = 1, Size alignment = 4);

    void Free(VoidPtr ptr);
    void FreeArray(VoidPtr ptr);

    void Reset();

    bool IsNull() const;

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


private:
    VoidPtr mainChunk_;
    Size mainChunkSize_;
    bool isOwner_;

    VoidPtr stackTopPtr_;
    Size currentStackScope_;


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
        VoidPtr allocationStart_;
        union
        {
            AllocHeaderSingle singleHeader_;
            AllocHeaderArray arrayHeader_;
        };

        AllocType type_;
    };
};


} // namespace Memory

} // namespace DXRL