#pragma once

#include <Foundation\Macro.hpp>
#include <Memory\Bytes.hpp>

namespace DXRL
{

namespace Memory
{

class LinearAllocator
{
public:
    DXRL_DEFINE_UNCOPYABLE_MOVABLE(LinearAllocator)

    LinearAllocator(VoidPtr chunk, Size size, bool isOwner = false);
    
    ~LinearAllocator();

    void* Alloc(Size size, Size alignment = 0);

    //template<typename T>
    //T* Alloc(Size count = 1, Size alignment = alignof(T))
    //{   
    //    return reinterpret_cast<T>(Alloc(CalculateSize(sizeof(T), alignment) * count, alignment);
    //}

    template<typename ALLOC>
    ALLOC* AllocChildAllocator(Size chunkSize)
    {
        return ALLOC{ Alloc(chunkSize), chunkSize, false };
    }

    void Reset();

private:
    Size CalculateSize(Size size, Size alignment = 0);

private:

    VoidPtr mainChunk_;
    Size mainChunkSize_;
    bool isOwner_;

    Size allocated_;
};

} // namespace Memory

} // namespace DXRL