#include <pch.hpp>
#include <chrono>
#include <random>
#include <bitset>

#include <Memory\Allocator.hpp>
#include <Container\Array.hpp>

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

int allocatormain()
{
    using namespace DXRL;
    
    char constexpr* testString{ "012345678901234567890123456789" };

    ///////////////////////////////////////////////////

    Memory::Mibibytes chunkSize = 1;
    Memory::VoidPtr mainChunk = malloc(chunkSize);
    Memory::FreeListAllocator alloc{ mainChunk, chunkSize, true };

    InplaceDynamicArray<CharBuffer40, 2, Memory::FreeListAllocator> container{ &alloc };
    
    Size_t a = alignof(CharBuffer40);

    for (Size_t i = 0; i < 4; ++i) {
        container.EmplaceBack();
    }
    
    for (Size_t i = 0; i < 2; ++i) {
        container.PopBack();
    }
    
    for (Size_t i = 0; i < 4; ++i) {
        container.EmplaceBack();
    }
    
    for (Size_t i = 0; i < 2; ++i) {
        container.PopBack();
    }

    system("pause");
    return 0;
}