#include <pch.hpp>
#include <chrono>
#include <random>
#include <bitset>

#include <Memory\Allocator.hpp>
#include <Memory\Pointer.hpp>

struct CharBuffer32
{
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

    Size constexpr allocationsCount = 10;
    
    Memory::Bytes linearChunkSize{ sizeof(CharBuffer32) * allocationsCount * 2 };
    Memory::VoidPtr linearMemoryChunk = malloc(linearChunkSize);
    Memory::LinearAllocator allocator{ linearMemoryChunk, linearChunkSize, true };
    
    for (Size i = 0; i < allocationsCount; ++i) {
        CharBuffer32* buffer = allocator.Alloc<CharBuffer32>();
        strcpy_s(buffer->content, sizeof(CharBuffer32), testString);
    }

    //Memory::VoidPtr ptr = reinterpret_cast<Memory::VoidPtr>(12);
    //Memory::VoidPtr alignedPtr = Memory::PtrAlign(ptr, 8);
    //
    //std::bitset<64> bitsetPtr{ reinterpret_cast<Memory::UintPtr>(ptr) };
    //std::bitset<64> bitsetAligned{ reinterpret_cast<Memory::UintPtr>(alignedPtr) };
    //std::cout << bitsetPtr << std::endl << bitsetAligned << std::endl;
    
    system("pause");
    return 0;
}