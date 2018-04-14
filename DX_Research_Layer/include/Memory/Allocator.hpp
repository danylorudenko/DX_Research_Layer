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

    template<typename T, typename U = U32>
    T* Alloc(U count = 1)
    {
        Size constexpr typeSize = sizeof(T);
        Size constexpr typeAlignment = alignof(T);

        VoidPtr const allocationResult = Alloc(typeSize * count, typeAlignment);
        return reinterpret_cast<T*>(allocationResult);
    }

    template<typename T, typename U = U32>
    VoidPtr Alloc(T size, U alignment = 0)
    {
        Size const sizeLocal = static_cast<Size>(size);
        Size const alignmentLocal = static_cast<Size>(size);

        return Alloc(sizeLocal, alignmentLocal);
    }

    template<typename TResult, typename T, typename U = U32, typename V = Size>
    TResult* AllocArray(T unitSize, U count = 1, V unitAlignment = alignof(TResult))
    {
        Size const unitSizeLocal = static_cast<Size>(unitSize);
        Size const localCount = static_cast<Size>(count);
        Size const unitAlignmentLocal = static_cast<Size>(unitAlignment);

        VoidPtr allocationResult = AllocArray(unitSizeLocal, unitAlignmentLocal, sizeLocal);
        return reinterpret_cast<TResult*>(allocationResult);
    }

    VoidPtr Alloc(Size size, Size alignment = 0);
    VoidPtr AllocArray(Size unitSize, Size count = 1, Size unitAlignment = 0);
    void Reset();

private:
    VoidPtr mainChunk_;
    Size mainChunkSize_;
    bool isOwner_;

    Size allocated_;
};


////////////////////////////////////////
class StackAllocator
{
public:
    DXRL_DEFINE_UNCOPYABLE_MOVABLE(StackAllocator)
    StackAllocator(VoidPtr chunk, Size size, bool isOwner = false);
    ~StackAllocator();
};


} // namespace Memory

} // namespace DXRL