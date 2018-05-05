#pragma once

#include <Foundation\Types.hpp>

namespace DXRL

{

////////////////////////////////////////
template<typename T, Size SIZE>
class Array
{
public:

    Array(Array<T, SIZE> const& rhs)
    {
        operator=(rhs);
    }

    Array(Array<T, SIZE>&& rhs)
    {
        operator=(std::move(rhs));
    }

    Array& operator=(Array<T, SIZE> const& rhs)
    {
        for (Size i = 0; i < SIZE; ++i) {
            rhs[i] = array_[i];
        }

        return *this;
    }

    Array& operator=(Array<T, SIZE>&& rhs)
    {
        for (Size i = 0; i < SIZE; ++i) {
            rhs[i] = std::move(array_[i]);
        }

        return *this;
    }

    DXRL::Size ArraySize() const { return SIZE; }

    T const& operator[](Size i) const { return array_[i]; }
    T& operator[](Size i) { return array_[i]; }

protected:
    T array_[SIZE];
};


////////////////////////////////////////
template<typename T, Size INPLACE_SIZE>
class InplaceArray : private Array<T, INPLACE_SIZE>
{
public:
    InplaceArray(InplaceArray<T, INPLACE_SIZE> const& rhs)
        : inplaceOverflow_{ rhs.inplaceOverflow_ }
        , size_{ rhs.size_ }
    {
        if (!inplaceOverflow_) {
            Array<T, INPLACE_SIZE>::operator=(rhs);
        }
        else {

        }
    }

protected:
    bool inplaceOverflow_;
    Size size_;
};


} // namespace DXRL