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

    //string0 += ("kek0");
    std::vector< CharBuffer40 > vector;
    std::vector< CharBuffer40 > vec2;

    vector = vec2;

    //string0 +=  string1;

    system("pause");
    return 0;
}