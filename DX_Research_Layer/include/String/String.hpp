#pragma once

#include <Container\Array.hpp>
#include <type_traits>

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

template<typename TChar>
Size CStringCopy(TChar const* source, TChar* dest, Size sourceSize)
{
    Size size = 0;
    while (size < sourceSize) {
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

    StaticBasicString(TChar const* str, Size sizeInMemory)
        : StaticArrayStorage<TChar, INPLACE_SIZE>{}
    {
        Size constexpr characterSize = sizeof(str[0]);
        Size const stringSize = sizeInMemory / characterSize;

        assert(stringSize < INPLACE_SIZE && "Can't fit the string in the internal storage.");

        _WrapData(str, stringSize);
    }

    template<
        typename TArg,
        typename = std::enable_if<
            !std::is_pointer<std::decay<TArg>::value>::value
        >::value
    >
    StaticBasicString(TArg&& str)
        : StaticArrayStorage<TChar, INPLACE_SIZE>{}
    {
        operator=(std::forward<TArg>(str));
    }
    
    StaticBasicString<TChar, INPLACE_SIZE>& operator=(TChar const* str)
    {
        Size size = CStringCopy<TChar>(str, Data());
        _ResizePure(size);

        return *this;
    }

    template<typename TArg>
    StaticBasicString<TChar, INPLACE_SIZE>& operator=(TArg&& str)
    {
        Size const size = str.GetSize();
        TChar* data = str.Data();
        _WrapData(data, size);

        return *this;
    }

    inline Size GetSize() const
    {
        return StaticArrayStorage<TChar, INPLACE_SIZE>::GetSize();
    }

    inline TChar const* Data() const
    {
        return StaticArrayStorage<TChar, INPLACE_SIZE>::Data();
    }

    inline TChar* Data()
    {
        return StaticArrayStorage<TChar, INPLACE_SIZE>::Data();
    }

    inline TChar operator[](Size i) const
    {
        return StaticArrayStorage<TChar, INPLACE_SIZE>::operator[](i);
    }

    inline TChar& operator[](Size i)
    {
        return StaticArrayStorage<TChar, INPLACE_SIZE>::operator[](i);
    }

};

}