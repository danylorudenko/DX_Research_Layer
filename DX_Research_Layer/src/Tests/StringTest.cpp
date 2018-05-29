#include <pch.hpp>
#include <chrono>
#include <random>
#include <bitset>

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
        DXRL::U64 contentInt_;
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

    StaticBasicString<char, 128> string0 = "kek";
    StaticBasicString<char, 128> string1 = "topKek";

    string0 += ("kek0");

    string0.operator+=( string1 );

    system("pause");
    return 0;
}