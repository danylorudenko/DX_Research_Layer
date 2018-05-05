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
    {
    }

    

private:
    VoidPtr data_;
    T* array_;

};

} // namespace DXRL