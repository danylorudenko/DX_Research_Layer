#pragma once

#include <Foundation\Types.hpp>

namespace DXRL

{
template<typename T>
class DynamicArray
{
    DynamicArray()
        : array_{ nullptr }
        , size_{ 0 }

    DynamicArray(T* array, Size size)
        : array_{ nullptr }
        , size_{ size }
    {
        
    }
private:
    T* array_;
    Size size_;
};

} // namespace DXRL