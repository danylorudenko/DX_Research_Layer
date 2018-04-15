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

inline VoidPtr PtrAdd(VoidPtr lhs, PtrDiff rhs)
{
    return reinterpret_cast<VoidPtr>(reinterpret_cast<UintPtr>(lhs) + static_cast<UintPtr>(rhs));
}

inline VoidPtr PtrAdd(VoidPtr lhs, UintPtr rhs)
{
    return reinterpret_cast<VoidPtr>(reinterpret_cast<UintPtr>(lhs) + rhs);
}


////////////////////////////////////////
template<typename T, typename U>
inline T* PtrAlign(T* ptr, U alignment)
{
    UintPtr ptrLocal = reinterpret_cast<UintPtr>(ptr);
    UintPtr alignmentLocal = static_cast<Size>(alignment);

    return reinterpret_cast<T*>((ptrLocal + alignmentLocal - 1) & (~alignmentLocal));
}

template<typename T, typename U>
inline UintPtr PtrAlignmentAdjustment(T* ptr, U alignment)
{
    UintPtr alignedPtr = reinterpret_cast<UintPtr>(PtrAlign(ptr, alignment));
    return alignedPtr - reinterpret_cast<UintPtr>(ptr);
}

template<typename T, typename U, typename V = S32>
inline T CalcSizeWithAlignment(T size, U alignment, V additionalDataSize = 0)
{
    return static_cast<T>(
        (static_cast<T>(alignment) > static_cast<T>(additionalDataSize))
        ? alignment
        : additionalDataSize
        );
}


}

}