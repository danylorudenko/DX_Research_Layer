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
        //DXRL::U64 contentInt[5];
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

    InplaceDynamicArray<CharBuffer40, 5, Memory::FreeListAllocator> container{ &alloc };
    //StaticArrayStorage<CharBuffer40, 20> container;

    Size kek = sizeof(container);

    for (Size i = 0; i < 10; ++i) {
        container.EmplaceBack();
    }

    for (Size i = 0; i < 7; ++i) {
        container.PopBack();
    }

    for (Size i = 0; i < 10; ++i) {
        container.EmplaceBack();
    }
    
    system("pause");
    return 0;
}