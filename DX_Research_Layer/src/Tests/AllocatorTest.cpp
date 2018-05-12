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
    Memory::FreeListAllocator alloc{ malloc(chunkSize), chunkSize, true };

    InplaceDynamicArray<CharBuffer40, 10, Memory::FreeListAllocator> arr{ &alloc, 5 };

    
    
    system("pause");
    return 0;
}