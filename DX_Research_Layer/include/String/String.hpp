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
template<typename TChar, Size_t SIZE>
class StaticBasicString
    : private StaticArrayStorage<TChar, SIZE>
{
public:
    StaticBasicString()
        : StaticArrayStorage<TChar, SIZE>{}
    { }

    StaticBasicString(TChar const* str)
        : StaticArrayStorage<TChar, SIZE>{}
    {
        operator=(str);
    }

    StaticBasicString(TChar const* str, Size_t sizeInMemory)
        : StaticArrayStorage<TChar, SIZE>{}
    {
        Size_t constexpr characterSize = sizeof(str[0]);
        Size_t const stringSize = sizeInMemory / characterSize;

        assert(stringSize < SIZE && "Can't fit the string in the internal storage.");

        _WrapData(str, stringSize);
    }

    template<
        typename TArg,
        typename = std::enable_if_t<
            !std::is_pointer<
                std::decay_t<TArg>
            >::value
        >
    >
    StaticBasicString(TArg&& str)
        : StaticArrayStorage<TChar, SIZE>{}
    {
        operator=(std::forward<TArg>(str));
    }
    
    StaticBasicString& operator=(TChar const* str)
    {
        Size_t size = CStringCopy<TChar>(str, Data());
        _ResizePure(size);

        return *this;
    }

    template<
        typename TArg,
        typename = std::enable_if_t<
            !std::is_pointer<
                std::decay_t<TArg>
            >::value
        >
    >
    StaticBasicString& operator=(TArg&& str)
    {
        Size_t const size = str.Size();
        TChar* data = str.Data();
        _WrapData(data, size);

        return *this;
    }

    inline TChar operator[](Size_t i) const
    {
        return StaticArrayStorage::operator[](i);
    }

    inline TChar& operator[](Size_t i)
    {
        return StaticArrayStorage::operator[](i);
    }

    inline Size_t Size() const
    {
        return StaticArrayStorage::Size();
    }

    inline TChar const* Data() const
    {
        return StaticArrayStorage::Data();
    }

    inline TChar* Data()
    {
        return StaticArrayStorage::Data();
    }

    StaticBasicString<TChar, INPLACE_SIZE>& operator+=(TChar const* str)
    {
        Size_t const newSize = CStringAppend(str, Data());
        _ResizePure(newSize);
        return *this;
    }

    template<
        typename TArg,
        typename = std::enable_if_t<
            !std::is_pointer<
                std::decay_t<TArg>
            >::value
        >
    >
    StaticBasicString<TChar, INPLACE_SIZE>& operator+=(TArg&& str)
    {
        TChar const* cStr = str.Data();
        Size_t const newSize = CStringAppend(str.Data(), Data());
        _ResizePure(newSize);
        return *this;
    }

};



////////////////////////////////////////
template<typename TChar, Size_t SIZE, typename TAllocator>
class InplaceBasicString
    : InplaceDynamicArray<TChar, INPLACE_SIZE, TAllocator>
{
    InplaceBasicString(TAllocator* allocator, TChar const* string)
        : InplaceDynamicArray<TChar, INPLACE_SIZE, TAllocator>{ allocator }
    {
        U32_fast_t counter = 0;
        while (string[counter] != CStringTerminator<TChar>()) {
            EmplaceBack(string[counter++]);
        }

        EmplaceBack(CStringTerminator<TChar>());
    }

    template<
        typename TArg,
        typename = std::enable_if_t<
            !std::is_pointer<
                std::decay_t<TArg>
            >::value
        >
    >
    InplaceBasicString(TArg&& arg)
    {
        operator=(std::forward<TArg>(arg));
    }

    InplaceBasicString& operator=(TChar const* src)
    {
        Clear();
        U32_fast_t counter = 0;
        while (string[counter] != CStringTerminator<TChar>()) {
            EmplaceBack(string[counter++]);
        }

        EmplaceBack(CStringTerminator<TChar>());

        return *this;
    }

    template<
        typename TArg,
        typename = std::enable_if_t<
            !std::is_pointer<
                std::decay_t<TArg>
            >::value
        >
    >
    InplaceBasicString& operator=(TArg&& rhs)
    {
        Clear();

        // Check storage requirements.
        auto const srcSize = rhs.Size() + 1;
        auto constexpr staticStorageSize = StaticStorageSize();
        if (srcSize > staticStorageSize) {
            auto const dynamicStorageSize = DynamicStorageSize();
            if (srcSize > dynamicStorageSize) {
                InplaceDynamicArray::ExpandStorage(srcSize);
            }
        }

        auto resultSize = CStringCopy(rhs.Data(), Data());
        _ResizePure(resultSize);

        return *this;
    }

    template<typename TArg>
    void EmpalceBack(TArg&& arg)
    {
        InplaceDynamicArray::EmplaceBack(std::forward<TArg>(arg));
    }

    InplaceBasicString& operator+=(TChar const* str)
    {
        U32_fast_t counter = 0;
        while (str[counter] != CStringTerminator<TChar>()) {
            EmplaceBack(str[counter++]);
        }
        EmplaceBack(CStringTerminator<TChar>());

        return *this;
    }

    TAllocator* Allocator()
    {
        return InplaceDynamicArray::Allocator();
    }

    inline bool IsDynamic() const
    {
        return InplaceDynamicArray::IsDynamic();
    }

    inline Size_t Size() const
    {
        return InplaceDynamicArray::Size();
    }

    inline Size_t StaticStorageSize() const
    {
        return InplaceDynamicArray::StaticStorageSize();
    }

    inline Size_t DynamicStorageSize() const
    {
        return InplaceDynamicArray::DynamicStorageSize();
    }

    inline void Clear()
    {
        InplaceDynamicArray::Clear();
        InplaceDynamicArray::EmplaceBack(CStringTerminator<TChar>());
    }

    inline TChar* Data()
    {
        return InplaceDynamicArray::Data();
    }

    inline TChar const* Data() const
    {
        return InplaceDynamicArray::Data();
    }


};

}