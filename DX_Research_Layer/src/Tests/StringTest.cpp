#include <pch.hpp>
#include <iostream>
#include <type_traits>

#include <Memory\Allocator.hpp>
#include <String\String.hpp>

struct CharBuffer40
{
    CharBuffer40()
    {
        strcpy_s(content, sizeof(CharBuffer40), "111111111111111111111111111111");
    }

    ~CharBuffer40()
    {
        strcpy_s(content, sizeof(CharBuffer40), "999999999999999999999999999999");
    }

    union
    {
        DXRL::U64_t contentInt_;
        char content[40];
    };
};

struct Uncopyable {

    Uncopyable()
    {

    }

    Uncopyable(Uncopyable const&) = delete;
    Uncopyable& operator=(Uncopyable const&) = delete;
    char content[32];
};

int main()
{
    using namespace DXRL;

    char constexpr* testString{ "012345678901234567890123456789" };

    ///////////////////////////////////////////////////

    Memory::Mibibytes chunkSize = 1;
    Memory::VoidPtr mainChunk = malloc(chunkSize);
    Memory::FreeListAllocator alloc{ mainChunk, chunkSize, true };

    //StaticBasicString<char, 128> string0 = "kek";
    //StaticBasicString<char, 128> string1 = string0;

    Array<CharBuffer40, 10> array_;
    Array<CharBuffer40, 10> array2_{ array_ };

    InplaceDynamicArray<CharBuffer40, 10, Memory::FreeListAllocator> inplaceArray{ &alloc };
    inplaceArray.EmplaceBack();
    inplaceArray.EmplaceBack(CharBuffer40{});

    auto size = inplaceArray.Size();
    //string0 +=  string1;

    system("pause");
    return 0;
}