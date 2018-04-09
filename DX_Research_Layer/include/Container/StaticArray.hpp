#pragma once

#include <Foundation\Types.hpp>
#include <Foundation\Macro.hpp>

namespace DXRL

{

template<typename T, Size SIZE>
class StaticArray
{
    StaticArray(T array[SIZE])
        : array_{ array }
    { }

    StaticArray(StaticArray const& rhs)
    {
        this->operator=(rhs);
    }

    StaticArray& operator=(StaticArray const& rhs)
    {
        for (DXRL::Size i = 0; i < SIZE; i++) {
            rhs[i] = array_[i];
        }

        return *this;
    }

    DXRL::Size Size() const { return SIZE; }

    T const& operator[](Size arg) const { return array_[arg]; }
    T& operator[](Size arg) { return array_[arg]; }

private:
    T array_[SIZE];
};

} // namespace DXRL