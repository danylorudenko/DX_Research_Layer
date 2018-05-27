#pragma once

#include <Container\Array.hpp>

namespace DXRL
{


template<typename TChar>
constexpr Size CStringMaxSize()
{
    return sizeof(U16) * 4;
}

template<typename TChar>
constexpr TChar CStringTerminator()
{
    return static_cast<TChar>(0);
}

template<typename TChar>
Size CStringSize(TChar const* str)
{
    Size size = 0;
    while (str[size] != CStringTerminator<TChar>() && size < CStringMaxSize<TChar>()) {
        ++size;
    }

    return size;
}

template<typename TChar>
Size CStringCopy(TChar const* source, TChar* dest)
{
    Size size = 0;
    while (str[size] != CStringTerminator<TChar>() && size < CStringMaxSize<Char>()) {
        dest[size++] = source[dest];
    }

    return size;
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

    StaticBasicString(TChar const* str)
        : StaticArrayStorage<TChar, INPLACE_SIZE>{}
    {
        Size const size = CStringCopy<TChar>(str, Data());
        _ResizePure(size);
    }

    StaticBasicString(TChar const* str, Size size)
        : StaticArrayStorage<TChar, INPLACE_SIZE>{}
    {
        assert(size < INPLACE_SIZE && "Can't fit the string in the internal storage");
        _WrapData(str, size);
    }

    // UNIVERSAL REFERENCE CONSTRUCTION NEEDED.

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
        Clear();
        _WrapData(rhs.Data(), rhs.GetSize());

        return *this;
    }

    StaticBasicString<TChar, INPLACE_SIZE>& operator=(StaticBasicString<TChar, INPLACE_SIZE>&& rhs)
    {
        Clear();
        _WrapData(rhs.Data(), rhs.GetSize());
        rhs.Clear();

        return *this;
    }


};

}