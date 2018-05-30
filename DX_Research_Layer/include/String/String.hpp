#pragma once

#include <Container\Array.hpp>
#include <type_traits>

namespace DXRL
{


template<typename TChar>
constexpr Size_t CStringMaxSize()
{
    return sizeof(U16_t) * 4;
}

template<typename TChar>
constexpr TChar CStringTerminator()
{
    return static_cast<TChar>(0);
}

template<typename TChar>
Size_t CStringSize(TChar const* str)
{
    Size_t size = 0;
    while (str[size] != CStringTerminator<TChar>() && size < CStringMaxSize<TChar>()) {
        ++size;
    }

    return size;
}

template<typename TChar>
Size_t CStringCopy(TChar const* source, TChar* dest)
{
    Size_t size = 0;
    while (str[size] != CStringTerminator<TChar>() && size < CStringMaxSize<Char>()) {
        dest[size++] = source[dest];
    }
    dest[size] = CStringTerminator<TChar>();

    return size;
}

template<typename TChar>
Size_t CStringCopy(TChar const* source, TChar* dest, Size_t sourceSize)
{
    Size_t size = 0;
    while (size < sourceSize) {
        dest[size++] = source[dest];
    }
    dest[size] = CStringTerminator<TChar>();

    return size;
}

template<typename TChar>
Size_t CStringAppend(TChar const* source, TChar* dest)
{
    Size_t destSize = CStringSize(dest);
    Size_t counter = 0;
    while (source[counter] != CStringTerminator<TChar>()) {
        dest[destSize + counter] = source[counter];
        ++counter;
    }

    Size_t const terminatorI = destSize + counter;
    dest[terminatorI] = CStringTerminator<TChar>();
    return terminatorI;
}

template<typename TChar>
void CStringTrunc(TChar* str, Size_t truncSize)
{
    str[truncSize] = CStringTerminator<TChar>();
}

////////////////////////////////////////
template<typename TChar, Size_t INPLACE_SIZE>
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
        operator=(str);
    }

    StaticBasicString(TChar const* str, Size_t sizeInMemory)
        : StaticArrayStorage<TChar, INPLACE_SIZE>{}
    {
        Size_t constexpr characterSize = sizeof(str[0]);
        Size_t const stringSize = sizeInMemory / characterSize;

        assert(stringSize < INPLACE_SIZE && "Can't fit the string in the internal storage.");

        _WrapData(str, stringSize);
    }

    template<
        typename TArg,
        typename = typename std::enable_if<
            !std::is_pointer<
                typename std::decay<TArg>::value
            >::value
        >::type
    >
    StaticBasicString(TArg&& str)
        : StaticArrayStorage<TChar, INPLACE_SIZE>{}
    {
        operator=(std::forward<TArg>(str));
    }
    
    StaticBasicString<TChar, INPLACE_SIZE>& operator=(TChar const* str)
    {
        Size_t size = CStringCopy<TChar>(str, Data());
        _ResizePure(size);

        return *this;
    }

    template<
        typename TArg,
        typename = typename std::enable_if<
            !std::is_pointer<
                typename std::decay<TArg>::value
            >::value
        >::type
    >
    StaticBasicString<TChar, INPLACE_SIZE>& operator=(TArg&& str)
    {
        Size_t const size = str.GetSize();
        TChar* data = str.Data();
        _WrapData(data, size);

        return *this;
    }

    inline Size_t GetSize() const
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

    StaticBasicString<TChar, INPLACE_SIZE>& operator+=(TChar const* str)
    {
        Size_t const newSize = CStringAppend(str, Data());
        _ResizePure(newSize);
        return *this;
    }

    template<
        typename TArg,
        typename = std::enable_if<
            !std::is_pointer<
                std::decay<TArg>::value
            >::value
        >::type
    >
    StaticBasicString<TChar, INPLACE_SIZE>& operator+=(TArg&& str)
    {
        TChar const* cStr = str.Data();
        Size_t const newSize = CStringAppend(str.Data(), Data());
        _ResizePure(newSize);
        return *this;
    }

    inline TChar operator[](Size_t i) const
    {
        return StaticArrayStorage<TChar, INPLACE_SIZE>::operator[](i);
    }

    inline TChar& operator[](Size_t i)
    {
        return StaticArrayStorage<TChar, INPLACE_SIZE>::operator[](i);
    }

};

}