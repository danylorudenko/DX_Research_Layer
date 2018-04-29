#pragma once

#include <Foundation\Types.hpp>

namespace DXRL
{

namespace Memory
{

////////////////////////////////////////
inline PtrDiff PtrDifference(VoidPtr lhs, VoidPtr rhs)
{
    return static_cast<PtrDiff>(reinterpret_cast<UintPtr>(lhs) - reinterpret_cast<UintPtr>(rhs));
}

inline VoidPtr PtrNegate(VoidPtr lhs, Size rhs)
{
    return reinterpret_cast<VoidPtr>(reinterpret_cast<UintPtr>(lhs) - static_cast<UintPtr>(rhs));
}

inline VoidPtr PtrAdd(VoidPtr lhs, Size rhs)
{
    return reinterpret_cast<VoidPtr>(reinterpret_cast<UintPtr>(lhs) + static_cast<UintPtr>(rhs));
}

////////////////////////////////////////
template<typename T, typename U>
inline T* PtrAlign(T* ptr, U alignment)
{
    UintPtr ptrLocal = reinterpret_cast<UintPtr>(ptr);
    UintPtr alignmentLocal = static_cast<UintPtr>(alignment);

    return reinterpret_cast<T*>((ptrLocal + alignmentLocal - 1) & (~(alignmentLocal - 1)));
}

template<typename T, typename U>
inline UintPtr PtrAlignmentAdjustment(T* ptr, U alignment)
{
    UintPtr alignedPtr = reinterpret_cast<UintPtr>(PtrAlign(ptr, alignment));
    return alignedPtr - reinterpret_cast<UintPtr>(ptr);
}

template<typename T, typename U, typename V = U32>
inline T CalcSizeWithAlignment(T size, U alignment, V headerSize = 0)
{
    return static_cast<T>(size + (alignment - 1) + headerSize);
}


}

}