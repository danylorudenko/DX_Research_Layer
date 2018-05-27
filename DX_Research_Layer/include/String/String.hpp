#pragma once

#include <Container\Array.hpp>

namespace DXRL
{

template<typename TChar>
static constexpr TChar StringTerminator()
{
    return static_cast<TChar>(0);
}

////////////////////////////////////////
template<typename TChar, Size INPLACE_SIZE>
class StaticBasicString
    : private StaticArrayStorage<TChar, INPLACE_SIZE>
{
public:
    StaticBasicString()
        : StaticArrayStorage<TChar, INPLACE_SIZE>{}
    { }

    template<typename... TArgs>
    StaticBasicString(TArgs&&... str)
        : StaticArrayStorage<TChar, INPLACE_SIZE>{}
    {
        
    }

    StaticBasicString(StaticBasicString<TChar, INPLACE_SIZE> const& rhs)
    {
        StaticBasicString<TChar, INPLACE_SIZE>::operator=(rhs);
    }

    StaticBasicString(StaticBasicString<TChar, INPLACE_SIZE>&& rhs)
    {
        StaticBasicString<TChar, INPLACE_SIZE>::operator=(std::move(rhs));
    }

    StaticBasicString<TChar, INPLACE_SIZE>& operator=(StaticBasicString<TChar, INPLACE_SIZE> const& rhs)
    {
        StaticArrayStorage<TChar, INPLACE_SIZE>::Clear();
        StaticArrayStorage<TChar, INPLACE_SIZE>::_WrapData(rhs.Data(), rhs.GetSize());

        return *this;
    }

    StaticBasicString<TChar, INPLACE_SIZE>& operator=(StaticBasicString<TChar, INPLACE_SIZE>&& rhs)
    {
        StaticArrayStorage<TChar, INPLACE_SIZE>::Clear();
        StaticArrayStorage<TChar, INPLACE_SIZE>::_WrapData(rhs.Data(), rhs.GetSize());
        rhs.Clear();

        return *this;
    }


};

}